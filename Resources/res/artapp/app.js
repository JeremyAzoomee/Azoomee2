define("app", [
    'angular',
    'angularRoute',
    'config',
    'views/artApp/artAppView',
    'rest/device'
], function(angular, angularRoute, config) {
     var app = angular.module(
            'azApp',
            [
                'ngRoute',
                'azApp.artApp',
                'com.tinizine.azoomee.rest.deviceApi'
            ]
        ).factory('connectivityHelper', ['$http','$rootScope','$timeout','deviceApi', function ($http, $rootScope, $timeout, deviceApi) {
             $rootScope.isOnline = true;
             var connectivityHelper = {
                 getIsOnline: function () {
                     return deviceApi.getIsOnline();
                 },
                 isOnline: function () {
                     return !connectivityHelper.isOffline();
                 },
                 isOffline: function () {
                     return $rootScope.isOnline === false;
                 }
             };
             var getOnlineStatus = function () {
                     connectivityHelper.getIsOnline().then(function (isOnline) {
                         $timeout(function () {
                             $rootScope.isOnline = isOnline;
                         });
                     });
                 },
                 routineCheck = function () {
                     getOnlineStatus();
                     setTimeout(routineCheck, config.offlineCheckInterval);   //We use setTimeout here instead of $timeout so as not to create a never-ending $timeout and protractor tests don't wait forever
                 };

             $(window).bind("online ononline offline onoffline", getOnlineStatus);
             routineCheck();
             return connectivityHelper;
         }])
         .config(['$routeProvider', function($routeProvider) {
             $routeProvider.otherwise({redirectTo: '/'});
         }])
        .controller('MainCtrl',
        [
            '$scope',
            '$rootScope',
            function($scope, $rootScope) {
                $rootScope.iOS = /iPad|iPhone|iPod/.test(navigator.userAgent) && !window.MSStream;
                $rootScope.safeApply = function(fn) {
                    var phase = this.$root.$$phase;
                    if(phase == '$apply' || phase == '$digest') {
                        return;
                    } else {
                        this.$apply();
                    }
                };
                return app;
            }
        ]
    );

    app.init = function () {
        console.log('init');
        function preload() {
            for (var i = 0; i < preload.arguments.length; i++) {
                $('#preload').prepend($('<img>',{id:'preload' + i,src:preload.arguments[i]}));
            }
        }
        preload(
            'style/images/artIcons/popup/button_down_proceed.png',
            'style/images/artIcons/gallery/art_button_down_new_painting.png'
        );
        require(['FastClick'], function (FastClick) {
            var initApp = function () {
                FastClick.attach(document.body, {});
                angular.bootstrap(document, ['azApp']);
            };
            initApp();
        });
    };
    return app;
});

