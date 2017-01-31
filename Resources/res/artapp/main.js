require.config({
    paths: {
        "angular": "bower_components/angular/angular",
        "angularRoute": "bower_components/angular-route/angular-route",
        "moment": "bower_components/moment/moment",
        "fabricjs": "bower_components/fabric.js/dist/fabric.require",
        "angular-animate": "bower_components/angular-animate/angular-animate",
        'angular-ellipsis': 'bower_components/angular-ellipsis/src/angular-ellipsis.min',
        'FastClick': 'bower_components/fastclick/lib/fastclick',
        'jquery': 'bower_components/jquery/dist/jquery.min',
        'appsflyer': 'bower_components/PhoneGap/www/appsflyer',
        'angular-lazy-img': 'bower_components/angular-lazy-img/release/angular-lazy-img.min',
        'hammer': 'bower_components/hammerjs/hammer.min'
    },
    packages: [
    ],
    shim: {
        'angular': {
            exports: 'angular'
        },
        'hammer': {
            exports: 'hammer'
        },
        'angularRoute': {
            exports: 'angularRoute',
            deps: [
                'angular'
            ]
        }
    },
    waitSeconds: 45,
    map: {
        '*': {
            //angular-strap.tpl requires angular-strap, but any module that requires angular-strap
            // will require both, so the simplest option is that angularStrap *means* angular-strap.tpl
            'angularStrap': 'angularStrapTpl'
        }
    }
});

require(['app',
], function (app) {
    app.init();
});