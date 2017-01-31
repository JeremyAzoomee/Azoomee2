define([
    'angular',
    'hammer',
    'config',
    'rest/device'
], function (angular, Hammer, config) {
    angular.module('azApp.artApp', ['ngRoute', 'angularAwesomeSlider'])
        .config(['$routeProvider', function ($routeProvider) {
            $routeProvider.when('/', {
                templateUrl: 'views/artApp/artApp.tmpl.html',
                controller: 'ArtAppCtrl'
            });
        }])
        .controller('ArtAppCtrl',
            ['$scope', '$rootScope', '$http', '$routeParams', '$location', 'connectivityHelper', '$timeout', 'deviceApi',
                function ($scope, $rootScope, $http, $routeParams, $location, connectivityHelper, $timeout, deviceApi) {
                    function saveToDevice(datURL) {
                        //console.log('save to device');
                        ////console.log(datURL);
                        //console.log("**********");
                        // if (controller.previousImage == null) {
                        //     console.log('Canvas is still blank');
                        //
                        // } else {
                        console.log('Save Canvas to LocalStorage');
                        localStorage['art'] = datURL;
                        window.parent.postMessage('saveImage', '*');
                        //   }

                    }

                    var checkImageTimer = 0;
                    var isCanvasBlank = true;

                    function checkForCanvas() {
                        //console.log('checking for canvas');
                        var currentArt = localStorage.getItem('art');
                        $scope.randomo = currentArt.length + "--" + Math.random().toString();
                        $rootScope.safeApply(null);
                        if (currentArt == "NEW") {
                            clearInterval(checkImageTimer);
                            controller.makeNewArt();
                        } else if (currentArt.length > 10000) {
                            clearInterval(checkImageTimer);
                            controller.loadSavedArt(currentArt);
                        }
                        ;
                    }

                    checkImageTimer = setInterval(checkForCanvas, 1000);
                    var controller = {
                        saveName: 'artApp',
                        stickers: {},
                        sprayIntervalValue: 1,
                        selectedBrush: 5,
                        currentTool: 'marker',
                        selectedColor: '#000000',
                        isDrawing: false,
                        setupSliders: function () {
                            $scope.sizeSlider = {
                                value: 10,
                                options: {
                                    from: 50,
                                    to: 1,
                                    step: 1,
                                    vertical: true,
                                    limits: false
                                }
                            };
                            $scope.$watch('sizeSlider.value', function () {
                                controller.sizeSelected($scope.sizeSlider.value);
                            });
                        },
                        init: function () {
                            $scope.showGallery = false;
                            //controller.showGallery();
                            $scope.selectedPattern = null;
                            $scope.patterns = {};
                            controller.setupScopeFunctionBindings();
                            $scope.sharingOverlayShown = false;
                            controller.scale = 1;
                            this.setBrushScopeVariables();
                            $scope.canvasWidth = $('#artApp').width();
                            $scope.userInput = {};
                            $scope.canvasHeight = $('#artApp').height();
                            $scope.itemWidth = Math.floor(($('#artApp').width() - 2) / 25);
                            controller.getHtmlElementVariables();
                            controller.loadPatterns();
                            controller.setColourScopeVariables();
                            $scope.drawingMode = false;
                            controller.setupSliders();
                            controller.bindEvents();
                            $scope.stickerPlacement = false;
                            $scope.colourOverlayShown = false;
                            $scope.stickerOverlayShown = false;
                            $scope.textOverlayShown = false;
                            controller.isPinching = false;
                            $scope.showDeleteArtworkConfirmationOverlay = controller.showDeleteArtworkConfirmationOverlay;
                            $scope.closeArtworkDeleteConfirm = controller.closeArtworkDeleteConfirm;
                            $scope.$on("$destroy", function () {
                                controller.destroy();
                            });
                            controller.offlineWatcherUnregister = $rootScope.$watch('isOnline', function () {
                                $scope.isOffline = connectivityHelper.isOffline();
                            }, true);
                            $scope.isOffline = connectivityHelper.isOffline();

                        },
                        setupScopeFunctionBindings: function () {
                            $scope.sizeSelected = controller.sizeSelected;
                            $scope.eraserSelected = controller.eraserSelected;
                            $scope.clearCanvas = controller.clearCanvas;
                            $scope.cancelSticker = controller.cancelSticker;
                            $scope.placeSticker = controller.placeSticker;
                            $scope.toggleColourSelector = controller.toggleColourSelector;
                            $scope.toggleStickerSelector = controller.toggleStickerSelector;
                            $scope.showText = controller.showText;
                            $scope.clearConfirmed = controller.clearConfirmed;
                            $scope.closeOverlay = controller.closeOverlay;
                            $scope.confirmText = controller.confirmText;
                            $scope.toolSelected = controller.toolSelected;
                            $scope.showStickers = controller.showStickers;
                            $scope.backClicked = controller.backClicked;
                            $scope.showColour = controller.showColour;
                            $scope.textColourSelected = controller.textColourSelected;
                            $scope.loadSharing = controller.loadSharing;
                            $scope.loadSavedArt = controller.loadSavedArt;
                            $scope.makeNewArt = controller.makeNewArt;
                            $scope.binToolClicked = controller.binToolClicked;
                            $scope.patternSelected = controller.patternSelected;
                            $scope.stickerSelected = controller.stickerSelected;
                            $scope.colorSelected = controller.colorSelected;
                            $scope.Math = window.Math;
                        },
                        getHtmlElementVariables: function () {
                            controller.$sketchCanvas = $('#colors_sketch');
                            controller.$sketchCanvasContext = controller.$sketchCanvas[0].getContext('2d');
                            controller.$baseCanvas = $('#baseCanvas');
                            controller.$baseCanvasContext = controller.$baseCanvas[0].getContext('2d');
                            controller.$stickerCanvas = $('#stickerCanvas');
                            controller.$stickerCanvasContext = controller.$stickerCanvas[0].getContext('2d');
                        },
                        setColourScopeVariables: function () {
                            $scope.textColours = config.textColours;
                            $scope.selectedTextColour = $scope.textColours[0];
                            $scope.colors = config.drawingColours.map(function (color, id) {
                                return {
                                    color: color,
                                    class: id === 0 ? 'selected' : ''
                                };
                            });
                            var colCount = Math.ceil($scope.colors.length / 2),
                                patternsCount = Math.ceil($scope.patternsCount / 2),
                                itemWidth = controller.calculateItemSize(),
                                paddingWidth = Math.ceil(0.08 * $(window).height());
                            $scope.colorScrollerWidth = (colCount * Math.ceil(itemWidth)) + paddingWidth;
                            $scope.patternsScrollerWidth = (patternsCount * Math.ceil(itemWidth)) + paddingWidth;
                            controller.selectedColor = '#' + $scope.colors[0].color;
                            $scope.selectedColor = controller.selectedColor;
                        },
                        calculateItemSize: function () {
                            var windowHeight = $(window).height(),
                                scrollerPaddingSize = Math.floor(0.08 * windowHeight),
                                scrollerHeight = Math.floor(windowHeight * 0.502),
                                availiableSize = scrollerHeight - scrollerPaddingSize,
                                itemSize = Math.floor(availiableSize * 0.44),
                                paddingSize = availiableSize * 0.03;
                            $scope.itemPadding = paddingSize;
                            $scope.itemSize = itemSize;
                            return availiableSize * 0.5;
                        },
                        setBrushScopeVariables: function () {
                            $scope.brushes = config.brushSizes.map(function (brush, id) {
                                return {
                                    size: brush,
                                    class: id === 0 ? 'selected' : ''
                                };
                            });
                            controller.selectedBrush = $scope.brushes[0].size;
                        },
                        loadSharing: function () {
                            //console.log("loadsharing");
                            var output = controller.$baseCanvas[0].toDataURL();
                            // //console.log(output);
                            //deviceApi.shareArtwork(controller.$baseCanvas[0].toDataURL());
                        },
                        undoState: null,
                        binToolClicked: function () {
                            if (controller.previousImage !== null) {
                                var imageObj = new Image();
                                imageObj.onload = function () {
                                    controller.clearCanvas(controller.$baseCanvas);
                                    controller.$baseCanvasContext.drawImage(this, 0, 0);
                                    controller.previousImage = null;
                                    $scope.undoDisplayState = 'bin';
                                    $scope.safeApply();
                                    controller.saveArt();
                                };
                                imageObj.src = controller.previousImage;
                                saveToDevice(controller.previousImage);
                                return;
                            }
                            controller.showClearConfirm();
                        },
                        showDeleteArtworkConfirmationOverlay: function (selectedArtwork) {
                            controller.artworkForDelete = selectedArtwork;
                            $scope.confirmDeleteOverlayShown = true;
                        },
                        closeArtworkDeleteConfirm: function (shouldRemove) {
                            if (shouldRemove) {
                                controller.removeSavedArtwork(controller.artworkForDelete);
                            }
                            controller.artworkForDelete = null;
                            $scope.confirmDeleteOverlayShown = false;
                        },
                        removeSavedArtwork: function (selectedArtwork) {
                            $scope.artworks = $scope.artworks.filter(function (artwork) {
                                return artwork.timestamp !== selectedArtwork.timestamp;
                            });
                            controller.writeToSaveFile($scope.artworks, function (success) {
                                //console.log('file delete save status was ' + success);
                            });
                        },
                        showGallery: function () {
                            $scope.showGallery = true;
                            $scope.safeApply();
                            controller.readSavedFile(function (artworks) {
                                artworks = artworks || [];
                                $scope.artworks = artworks.sort(function (a, b) {
                                    if (a.timestamp > b.timestamp) {
                                        return -1;
                                    }

                                    if (a.timestamp < b.timestamp) {
                                        return 1;
                                    }
                                    return 0;
                                });
                                $scope.safeApply();
                            });
                        },
                        saveArt: function (insertNew, callback) {
                            //console.log('save current status');
                            var currentArt = {
                                artWork: controller.$baseCanvas[0].toDataURL(),
                                timestamp: (new Date()).getTime()
                            };
                            saveToDevice(currentArt.artWork);
                            controller.readSavedFile(function (existingSaves) {
                                if (!existingSaves) {
                                    existingSaves = [];
                                }
                                if (insertNew) {
                                    existingSaves.unshift(currentArt);
                                    if (existingSaves.length > 9) {
                                        existingSaves.length = 9;
                                    }
                                } else {
                                    existingSaves[0] = currentArt;
                                }
                                controller.writeToSaveFile(existingSaves, function (success) {
                                    //console.log('file save status was ' + success);
                                    if (callback) {
                                        callback(true);
                                    }
                                });
                            });
                        },
                        makeNewArt: function () {
                            $scope.showGallery = false;
                            $scope.artworks = null;
                            controller.previousImage = null;
                            $scope.undoDisplayState = 'bin';
                            controller.saveArt(true);
                        },
                        loadSavedArt: function (artwork) {
                            //console.log('SAVED ART');
                            //console.log(artwork.length);
                            $scope.showGallery = false;
                            controller.previousImage = null;
                            $scope.undoDisplayState = 'bin';
                            var imageObj = new Image();
                            imageObj.onload = function () {
                                controller.$baseCanvasContext.drawImage(this, 0, 0);
                                controller.saveArt(true);
                            };
                            imageObj.src = artwork;
                            $scope.artworks = null;
                        },
                        readSavedFile: function (callback) {
                            callback = callback || function () {
                                };
                            deviceApi.getSavedArt().then(function (savedArt) {
                                callback(savedArt);
                            });
                        },
                        writeToSaveFile: function (data, callback) {
                            callback = callback || function () {
                                };
                            deviceApi.saveArt(data).then(function (savedArt) {
                                callback(savedArt);
                            });
                        },
                        backClicked: function () {
                            if ($scope.showGallery === true) {
                                deviceApi.exitApp();
                                return;
                            }
                            controller.cancelSticker();
                            controller.saveArt(false, function () {
                                //console.log('saved...');
                                //console.log('showing gallery');
                                controller.showGallery();
                                controller.clearCanvas(controller.$baseCanvas);
                                controller.clearCanvas(controller.$sketchCanvas);
                                controller.clearCanvas(controller.$stickerCanvas);
                            });
                        },
                        setColourItemSize: function () {
                            var availableHeight = (($('.contentHolder').height() * 0.378) / 2) - 30;
                            $scope.colourItemSize = Math.floor(availableHeight);
                        },
                        showColour: function () {
                            clearInterval(controller.sprayInterval);
                            controller.setColourItemSize();

                            $scope.colourOverlayShown = true;
                            $scope.showColourPanelId = 0;
                            $scope.stickerOverlayShown = false;
                        },
                        showClearConfirm: function () {
                            $scope.clearConfirmOverlayShown = true;
                        },
                        clearConfirmed: function () {
                            controller.clearCanvas(controller.$baseCanvas);
                            controller.saveArt();
                            controller.closeOverlay();
                        },
                        toggleColourSelector: function (panelId) {
                            $scope.showColourPanelId = panelId;
                        },
                        loadPatterns: function () {
                            var crayonTexture = new Image(),
                                stickerPacks = config.stickerPacks,
                                patterns = config.patterns,
                                x,
                                y;

                            $scope.patternsCount = patterns.length;
                            for (x = 0; x < patterns.length; x++) {
                                controller.loadPattern(patterns[x]);
                            }
                            crayonTexture.onload = function () {
                                var pattern = controller.$baseCanvasContext.createPattern(crayonTexture, 'repeat');
                                controller.crayonPattern = pattern;
                            };
                            crayonTexture.src = 'style/images/crayon-texture2.png';

                            $scope.stickerPacks = [];
                            $scope.stickerUrls = stickerPacks;
                            for (x = 0; x < stickerPacks.length; x++) {
                                $scope.stickerPacks[x] = {
                                    name: stickerPacks[x].name,
                                    stickers: []
                                };
                                for (y = 0; y < stickerPacks[x].stickers.length; y++) {
                                    controller.loadImage(x, y, stickerPacks[x].stickers[y]);
                                }
                            }
                        },
                        loadPattern: function (pattern) {
                            var patternImage = new Image();
                            patternImage.onload = function () {
                                var canvasPattern = controller.$baseCanvasContext.createPattern(patternImage, 'repeat');

                                $scope.patterns[pattern.name] = canvasPattern;
                                $scope.patterns[pattern.name].displayImage = patternImage;
                            };
                            patternImage.src = pattern.url;
                        },
                        loadImage: function (packId, stickerId, url) {
                            var stickerImage = new Image();
                            stickerImage.onload = function () {
                                $scope.stickerPacks[packId].stickers[stickerId] = stickerImage;
                            };
                            stickerImage.src = url;
                        },
                        toggleStickerSelector: function (index) {
                            $scope.selectedStickerPanel = index;
                        },
                        bindEvents: function () {
                            controller.$baseCanvas.on('mousedown touchstart', controller.canvasTouched);
                            controller.$sketchCanvas.on('mousedown touchstart', controller.canvasTouched);
                            controller.$baseCanvas.on('mousemove touchmove', controller.mouseMove);
                            controller.$sketchCanvas.on('mousemove touchmove', controller.mouseMove);
                            $('.tools').on('mousemove touchmove', function (e) {
                                e.preventDefault();
                            });
                            controller.$baseCanvas.on('mouseup touchend touchcancel mouseout mouseleave', controller.mouseEnd);
                            controller.$sketchCanvas.on('mouseup touchend touchcancel mouseout mouseleave', controller.mouseEnd);
                            $scope.$on('$destroy', function () {
                                if (!$scope.showGallery) {
                                    controller.saveArt(false);
                                }
                                controller.unbindEvents();
                            });
                        },
                        unbindEvents: function () {
                            controller.$sketchCanvas.off('mouseup touchend touchcancel mouseout mouseleave mousemove touchmove mousedown touchstart');
                            controller.$baseCanvas.off('mouseup touchend touchcancel mouseout mouseleave mousemove touchmove mousedown touchstart');
                            controller.removeHammerTime();
                            $('.tools').off('mousemove touchmove');
                        },
                        mouseEnd: function (e) {
                            if (e && e.originalEvent && e.originalEvent.targetTouches && e.originalEvent.targetTouches.length > 0) {
                                return;
                            }
                            clearInterval(controller.sprayInterval);
                            if (controller.isDrawing) {
                                controller.flattenCanvas();
                            }
                            controller.isDrawing = false;
                            //console.log('mouseend');
                            saveToDevice(controller.$baseCanvas[0].toDataURL());

                        },
                        mouseMove: function (e) {
                            e.preventDefault();
                            var touchPos = controller.calculateTouchPositionFromEvent(e);
                            controller.mousePos = touchPos;
                            if (!controller.isDrawing) {
                                return;
                            }
                            controller.draw[controller.currentTool](touchPos);
                        },
                        clearCanvas: function ($canvasDomElement) {
                            $canvasDomElement = $canvasDomElement || controller.$baseCanvas;
                            var canvasContext = $canvasDomElement[0].getContext('2d');
                            canvasContext.clearRect(0, 0, $canvasDomElement.width(), $canvasDomElement.height());
                        },
                        draw: {
                            highlighter: function (touchPos, pattern, opacity) {
                                var x, canvasContext = controller.$sketchCanvasContext;
                                opacity = opacity || 0.5;
                                controller.previousEvents.push(touchPos);
                                controller.clearCanvas(controller.$sketchCanvas);
                                canvasContext.lineJoin = "round";
                                canvasContext.lineCap = "round";

                                canvasContext.globalAlpha = opacity;
                                canvasContext.beginPath();
                                canvasContext.moveTo(controller.previousEvents[0].x, controller.previousEvents[0].y);
                                if (controller.previousEvents === 1) {
                                    canvasContext.moveTo(controller.previousEvents[0].x - 1, controller.previousEvents[0].y);
                                }
                                for (x = 1; x < controller.previousEvents.length; x++) {
                                    canvasContext.lineTo(controller.previousEvents[x].x, controller.previousEvents[x].y);

                                }
                                canvasContext.strokeStyle = $scope.selectedPattern || controller.selectedColor;
                                canvasContext.lineWidth = controller.selectedBrush;
                                canvasContext.stroke();
                                if (pattern) {
                                    canvasContext.save();
                                    canvasContext.globalCompositeOperation = "destination-out";
                                    canvasContext.fillStyle = pattern;
                                    canvasContext.translate(controller.overlayOffset.x, controller.overlayOffset.y);
                                    canvasContext.fillRect(-controller.overlayOffset.x, -controller.overlayOffset.y, controller.$sketchCanvas.width(), controller.$sketchCanvas.height());
                                    canvasContext.restore();
                                }
                                canvasContext.globalCompositeOperation = "source-over";
                                canvasContext.globalAlpha = 1;
                                return;
                            },
                            crayon: function (touchPos) {
                                controller.draw.highlighter(touchPos, controller.crayonPattern, 1);
                            },
                            marker: function (touchPos) {
                                var canvasContext = controller.$baseCanvasContext, previous = controller.lastMousePos;
                                canvasContext.lineJoin = "round";
                                canvasContext.lineCap = "round";
                                canvasContext.beginPath();
                                if (!controller.lastMousePos) {
                                    canvasContext.moveTo(touchPos.x - 1, touchPos.y);
                                } else {
                                    canvasContext.moveTo(previous.x, previous.y);
                                }
                                canvasContext.lineTo(touchPos.x, touchPos.y);
                                canvasContext.strokeStyle = $scope.selectedPattern || controller.selectedColor;
                                canvasContext.lineWidth = controller.selectedBrush;
                                controller.lastMousePos = touchPos;
                                return canvasContext.stroke();
                            },
                            circle: function (touchPos) {
                                var canvasContext = controller.$baseCanvasContext, previous = controller.lastMousePos, opacity, distance,
                                    radius = controller.selectedBrush;
                                distance = 0;
                                if (previous) {
                                    var previousDistanceX = Math.abs(touchPos.x - previous.x),
                                        previousDistanceY = Math.abs(touchPos.y - previous.y);
                                    distance = Math.sqrt((previousDistanceX * previousDistanceX) + (previousDistanceY * previousDistanceY));
                                }
                                distance = distance < 100 ? distance : 100;
                                opacity = 1 - (0.9 * (distance / 100));
                                canvasContext.globalAlpha = opacity;
                                canvasContext.beginPath();
                                canvasContext.arc(touchPos.x, touchPos.y, radius, 0, 2 * Math.PI, false);

                                canvasContext.fillStyle = $scope.selectedPattern || controller.selectedColor;
                                canvasContext.fill();
                                canvasContext.globalAlpha = 1;
                                controller.lastMousePos = touchPos;
                            },
                            eraser: function (touchPos) {
                                var canvasContext = controller.$baseCanvasContext,
                                    x = touchPos.x,
                                    y = touchPos.y;
                                if (controller.lastMousePos) {
                                    x = controller.lastMousePos.x;
                                    y = controller.lastMousePos.y;
                                }
                                canvasContext.beginPath();
                                canvasContext.lineWidth = controller.selectedBrush;
                                canvasContext.lineJoin = "round";
                                canvasContext.lineCap = "round";
                                canvasContext.globalCompositeOperation = "destination-out";
                                canvasContext.moveTo(x, y);
                                canvasContext.lineTo(touchPos.x, touchPos.y);
                                canvasContext.stroke();
                                controller.lastMousePos = touchPos;
                                canvasContext.globalCompositeOperation = "source-over";
                            }
                        },
                        getRandomOffset: function (radius) {
                            var random_angle = Math.random() * (2 * Math.PI), random_radius = Math.random() * radius;
                            return {
                                x: Math.cos(random_angle) * random_radius,
                                y: Math.sin(random_angle) * random_radius
                            };
                        },
                        drawSpray: function () {
                            var spraySize = (parseInt(controller.selectedBrush) + 30) / 2;
                            controller.$baseCanvasContext.fillStyle = $scope.selectedPattern || controller.selectedColor;
                            for (var i = 0; i < spraySize; i++) {
                                var offset = controller.getRandomOffset(spraySize),
                                    x = controller.mousePos.x + offset.x,
                                    y = controller.mousePos.y + offset.y;
                                controller.$baseCanvasContext.fillRect(x, y, 1, 1);
                            }
                        },
                        colorSelected: function (colour) {
                            $scope.selectedPattern = null;
                            controller.selectedColor = '#' + colour;
                            $scope.selectedColor = controller.selectedColor;
                            $scope.colourOverlayShown = false;
                            return true;
                        },
                        closeOverlay: function () {
                            $scope.clearConfirmOverlayShown = false;
                            $scope.typingTool = false;
                            $scope.colourOverlayShown = false;
                            $scope.textOverlayShown = false;
                            $scope.stickerOverlayShown = false;
                            $scope.sharingOverlayShown = false;
                            $scope.friends = null;
                            return true;
                        },
                        patternSelected: function (pattern) {
                            $scope.selectedPattern = pattern;
                            $scope.colourOverlayShown = false;
                            return true;
                        },
                        sizeSelected: function (brush) {
                            controller.selectedBrush = brush;
                            return true;
                        },
                        toolSelected: function (tool) {
                            clearInterval(controller.sprayInterval);
                            $('.tool').removeClass('selected');
                            $('.' + tool).addClass('selected');
                            $scope.drawingMode = false;
                            $scope.drawingMode = tool === "highlighter" ? true : false;
                            $scope.drawingMode = tool === "crayon" ? true : $scope.drawingMode;
                            controller.currentTool = tool;
                            $scope.stickerPlacement = false;
                        },
                        stickerSelected: function (packId, stickerId, sticker) {
                            sticker = sticker || $scope.stickerPacks[packId].stickers[stickerId];
                            $scope.stickerOverlayShown = false;
                            controller.currentSticker = sticker;
                            $scope.drawingMode = false;
                            controller.scale = 1;
                            controller.currentTool = 'sticker';
                            controller.stickerRotation = 360;
                            $scope.stickerPlacement = true;
                            controller.bindHammerTime();
                            controller.stickerPos = {
                                x: controller.$stickerCanvas[0].width / 2,
                                y: controller.$stickerCanvas[0].height / 2
                            };
                            controller.drawSticker(controller.stickerPos.x, controller.stickerPos.y, controller.currentSticker);
                        },
                        handleGesture: function (event) {
                            if (event.type === "rotatestart") {
                                controller.previousRotation = event.rotation % 360;
                                controller.rotateInProgress = true;
                                controller.initialScale = controller.scale;
                                return;
                            }
                            if (event.type === "rotateend" || event.type === "rotatecancel") {
                                controller.disableTapAfterGesture();
                                return;
                            }
                            if (event.type === "pinchstart") {
                                controller.pinchInProgress = true;
                                controller.initialScale = controller.scale;
                                return;
                            }
                            if (event.type === "pinchend" || event.type === "pinchcancel") {
                                controller.disableTapAfterGesture();
                                return;
                            }
                            if (event.type === "rotate") {
                                var eventRotation = event.rotation % 360,
                                    rotationDiff = eventRotation - controller.previousRotation,
                                    newRotation = controller.stickerRotation + rotationDiff,
                                    absDifferent = Math.abs(rotationDiff);

                                if ((absDifferent > 30 && absDifferent && absDifferent < 350) || absDifferent > 370) {
                                    return;
                                }
                                controller.stickerRotation = newRotation;
                                controller.previousRotation = eventRotation;
                            }
                            controller.stickerPos = event.center;
                            controller.resizeSticker(event.scale * controller.initialScale);
                        },
                        disableTapAfterGesture: function () {
                            $timeout(function () {
                                controller.rotateInProgress = false;
                                controller.pinchInProgress = false;
                            }, 150);
                        },
                        handleTap: function (event) {

                            if (controller.pinchInProgress || controller.rotateInProgress) {
                                return;
                            }
                            controller.stickerPos = event.center;
                            controller.drawSticker(controller.stickerPos.x, controller.stickerPos.y, controller.currentSticker);
                        },
                        showStickers: function (selectedStickerPanel) {
                            selectedStickerPanel = selectedStickerPanel || $scope.stickerUrls[0].name;
                            clearInterval(controller.sprayInterval);
                            controller.setColourItemSize();
                            $scope.selectedStickerPanel = selectedStickerPanel;
                            $scope.textOverlayShown = false;
                            $scope.colourOverlayShown = false;
                            $scope.stickerOverlayShown = true;
                        },
                        flattenCanvas: function () {
                            if (!controller.isDrawing) {
                                return;
                            }
                            controller.$baseCanvasContext.drawImage(controller.$sketchCanvas[0], 0, 0);
                            controller.clearCanvas(controller.$sketchCanvas);
                        },
                        calculateTouchPositionFromEvent: function (e) {
                            if (e.originalEvent.targetTouches) {
                                e.pageX = e.originalEvent.targetTouches[0].pageX;
                                e.pageY = e.originalEvent.targetTouches[0].pageY;
                            }
                            return {
                                x: e.pageX - controller.$baseCanvas.offset().left,
                                y: e.pageY - controller.$baseCanvas.offset().top
                            };
                        },
                        cancelSticker: function () {
                            controller.currentSticker = null;
                            $scope.stickerPlacement = false;
                            controller.removeHammerTime();
                            controller.toolSelected('marker');
                            controller.clearCanvas(controller.$sketchCanvas);
                        },
                        placeSticker: function () {
                            controller.currentSticker = null;
                            $scope.stickerPlacement = false;
                            controller.toolSelected('marker');
                            controller.removeHammerTime();
                            controller.previousImage = controller.$baseCanvas[0].toDataURL();

                            $scope.undoDisplayState = 'undo';
                            controller.$baseCanvasContext.drawImage(controller.$stickerCanvas[0], 0, 0);
                            controller.clearCanvas(controller.$stickerCanvas);
                            //console.log('placeSticker');
                            saveToDevice(controller.$baseCanvas[0].toDataURL());
                        },
                        bindHammerTime: function () {
                            controller.hammertime = new Hammer(controller.$stickerCanvas[0]);
                            controller.hammertime.get('pinch').set({enable: true});
                            controller.hammertime.get('rotate').set({enable: true});
                            controller.hammertime.get('pan').set({direction: Hammer.DIRECTION_ALL});
                            controller.hammertime.on('pinch rotate rotatestart rotateend rotatecancel pinchstart pinchend pinchcancel', function (ev) {
                                controller.handleGesture(ev);
                            });
                            controller.hammertime.on('tap press pan', function (ev) {
                                controller.handleTap(ev);
                            });
                        },
                        removeHammerTime: function () {
                            if (!controller.hammertime) {
                                return;
                            }
                            controller.hammertime.off('pinch');
                            controller.hammertime.off('rotate');
                            controller.hammertime.off('rotatestart');
                            controller.hammertime.off('tap');
                            controller.isPinching = false;
                            controller.hammertime.destroy();
                            controller.hammertime = undefined;
                        },
                        resizeSticker: function (scale) {
                            if (!controller.stickerPos) {
                                return;
                            }
                            if (scale < 0.05) {
                                scale = 0.05;
                            }
                            if (scale > 3) {
                                scale = 3;
                            }
                            controller.scale = scale;
                            controller.drawSticker(controller.stickerPos.x, controller.stickerPos.y, controller.currentSticker);
                        },
                        confirmText: function () {
                            controller.clearCanvas(controller.$stickerCanvas);
                            controller.$stickerCanvasContext.font = "200px Arial Rounded MT Bold";
                            controller.$stickerCanvasContext.fillStyle = "#" + $scope.selectedTextColour;
                            controller.$stickerCanvasContext.textAlign = "center";
                            controller.$stickerCanvasContext.fillText($scope.userInput.text, controller.$stickerCanvas[0].width / 2, controller.$stickerCanvas[0].height / 2);

                            var url = controller.$stickerCanvas[0].toDataURL(),
                                stickerImage = new Image();
                            stickerImage.src = url;
                            controller.clearCanvas(controller.$stickerCanvas);
                            controller.stickerPos = null;
                            controller.stickerSelected(null, null, stickerImage);
                        },
                        canvasTouched: function (e) {
                            var multipleTouch = (e && e.originalEvent && e.originalEvent.targetTouches && e.originalEvent.targetTouches.length > 1);
                            e.preventDefault();
                            controller.previousImage = controller.$baseCanvas[0].toDataURL();
                            $scope.undoDisplayState = 'undo';
                            $scope.safeApply();
                            switch (controller.currentTool) {
                                case "spray":
                                    if (controller.sprayInterval) {
                                        clearInterval(controller.sprayInterval);
                                    }
                                    controller.sprayInterval = setInterval(controller.drawSpray, controller.sprayIntervalValue);
                                    controller.mouseMove(e);
                                    break;
                                case "highlighter":
                                case "crayon":
                                    if (controller.isDrawing && multipleTouch) {
                                        return;
                                    }
                                    controller.previousEvents = [];
                                    controller.overlayOffset = {
                                        x: Math.floor(Math.random() * 10),
                                        y: Math.floor(Math.random() * 10)
                                    };
                                    controller.isDrawing = true;
                                    controller.lastMousePos = null;
                                    controller.mouseMove(e);
                                    break;
                                case "circle":
                                case "marker":
                                case "eraser":
                                    controller.isDrawing = true;
                                    controller.lastMousePos = null;
                                    controller.mouseMove(e);
                                    break;
                            }
                            saveToDevice(controller.$baseCanvas[0].toDataURL());
                        },
                        drawSticker: function (x, y, image) {
                            controller.clearCanvas(controller.$stickerCanvas);
                            var ratio = controller.scale || 1,
                                newWidth = image.width * ratio,
                                newHeight = image.height * ratio,
                                angleInRadians = 0.0174533 * controller.stickerRotation;
                            controller.$stickerCanvasContext.translate(x, y);
                            controller.$stickerCanvasContext.rotate(angleInRadians);
                            controller.$stickerCanvasContext.drawImage(image, 0 - (0.5 * newWidth), 0 - (0.5 * newHeight), newWidth, newHeight);
                            controller.$stickerCanvasContext.rotate(-angleInRadians);
                            controller.$stickerCanvasContext.translate(-x, -y);
                        },
                        destroy: function () {
                            controller.offlineWatcherUnregister();
                        }
                    };
                    controller.init();
                    return controller;
                }
            ]
        );
});