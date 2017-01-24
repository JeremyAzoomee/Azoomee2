define([
    'angular'
], function(angular) {
    angular.module('azApp.devicePlatform', [])
        .factory("devicePlatform", function () {
            var iOS = /iPad|iPhone|iPod/.test(navigator.userAgent) && !window.MSStream;
            return iOS ? "ios" : "android";
        });
});