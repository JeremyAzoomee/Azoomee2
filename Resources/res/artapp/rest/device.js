define([
    'config',
    'angular',
], function (config, angular) {
    angular.module('com.tinizine.azoomee.rest.deviceApi', [])
        .factory('deviceApi', ['$q', function ($q) {
            var deviceApi = {
                exitApp: function () {
                    alert("Tell device to quit");
                },
                getIsOnline: function () {
                    var deferred = $q.defer();
                    deferred.resolve(true);
                    return deferred.promise;
                },
                getSavedArt: function () {
                    var deferred = $q.defer();
                    deferred.resolve([]);
                    return deferred.promise;
                },
                saveArt: function () {
                    var deferred = $q.defer();
                    deferred.resolve(true);
                    return deferred.promise;
                },
                shareArtwork: function (imageData) {
                    alert("Tell device to share artwork");
                    console.log(imageData);
                    var deferred = $q.defer();
                    deferred.resolve(true);
                    return deferred.promise;
                }
            };
            return deviceApi;
        }]);
});