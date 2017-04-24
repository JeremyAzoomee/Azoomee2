window.define("config", [], function() {
    return (function () {
        return {
            'offlineCheckInterval': 30000,
            'textColours': ['55CDC3', '36485C', 'FF4A5B', 'FFFFFF', 'EDEDDA'],
            'drawingColours': [
                'E40083',
                'DE0000',
                'E48900',
                'E8CA00',
                '1D9E44',
                '1BA0A3',
                '8D1789',
                '343436',
                '5F6161',
                'CDD1CD',
                'F2F2F2',
                'D4A47C',
                '88511E',
                '561C00'
            ],
            'brushSizes': [3, 5, 10, 15, 30, 50, 99],
            'stickerPacks': [
                {
                    name: 'space',
                    displayImage: 'style/images/stickers/art_sticker_01_unselected.png',
                    selectedDisplayImage: 'style/images/stickers/art_sticker_01_selected.png',
                    stickers: [
                        'style/images/stickers/stickers/01/space_01.svg',
                        'style/images/stickers/stickers/01/space_02.svg',
                        'style/images/stickers/stickers/01/space_03.svg',
                        'style/images/stickers/stickers/01/space_04.svg',
                        'style/images/stickers/stickers/01/space_05.svg',
                        'style/images/stickers/stickers/01/space_06.svg',
                        'style/images/stickers/stickers/01/space_07.svg',
                        'style/images/stickers/stickers/01/space_08.svg',
                        'style/images/stickers/stickers/01/vehic_01.svg',
                        'style/images/stickers/stickers/01/vehic_02.svg',
                        'style/images/stickers/stickers/01/vehic_03.svg',
                        'style/images/stickers/stickers/01/vehic_04.svg',
                        'style/images/stickers/stickers/01/vehic_05.svg',
                        'style/images/stickers/stickers/01/vehic_06.svg',
                        'style/images/stickers/stickers/01/vehic_07.svg',
                        'style/images/stickers/stickers/01/vehic_08.svg'
                    ]
                },
                {
                    name: 'animals',
                    displayImage: 'style/images/stickers/art_sticker_02_unselected.png',
                    selectedDisplayImage: 'style/images/stickers/art_sticker_02_selected.png',
                    stickers: [
                        'style/images/stickers/stickers/02/anim_01.svg',
                        'style/images/stickers/stickers/02/anim_02.svg',
                        'style/images/stickers/stickers/02/anim_03.svg',
                        'style/images/stickers/stickers/02/anim_04.svg',
                        'style/images/stickers/stickers/02/anim_05.svg',
                        'style/images/stickers/stickers/02/anim_06.svg',
                        'style/images/stickers/stickers/02/anim_07.svg',
                        'style/images/stickers/stickers/02/anim_08.svg',
                        'style/images/stickers/stickers/02/anim_09.svg',
                        'style/images/stickers/stickers/02/anim_10.svg',
                        'style/images/stickers/stickers/02/anim_11.svg',
                        'style/images/stickers/stickers/02/anim_14.svg',
                        'style/images/stickers/stickers/02/anim_17.svg',
                        'style/images/stickers/stickers/02/anim_18.svg',
                        'style/images/stickers/stickers/02/anim_19.svg',
                        'style/images/stickers/stickers/02/anim_20.svg',
                        'style/images/stickers/stickers/02/anim_21.svg',
                        'style/images/stickers/stickers/02/anim_22.svg',
                        'style/images/stickers/stickers/02/anim_23.svg',
                        'style/images/stickers/stickers/02/anim_25.svg',
                        'style/images/stickers/stickers/02/anim_26.svg',
                        'style/images/stickers/stickers/02/anim_28.svg',
                        'style/images/stickers/stickers/02/anim_30.svg',
                        'style/images/stickers/stickers/02/anim_31.svg',
                        'style/images/stickers/stickers/02/anim_32.svg',
                        'style/images/stickers/stickers/02/anim_33.svg',
                        'style/images/stickers/stickers/02/anim_34.svg',
                        'style/images/stickers/stickers/02/anim_35.svg',
                        'style/images/stickers/stickers/02/anim_36.svg',
                        'style/images/stickers/stickers/02/anim_37.svg',
                        'style/images/stickers/stickers/02/anim_39.svg',
                        'style/images/stickers/stickers/02/anim_40.svg',
                        'style/images/stickers/stickers/02/anim_43.svg',
                        'style/images/stickers/stickers/02/anim_47.svg',
                        'style/images/stickers/stickers/02/anim_48.svg',
                        'style/images/stickers/stickers/02/anim_49.svg',
                        'style/images/stickers/stickers/02/anim_50.svg',
                        'style/images/stickers/stickers/02/anim_51.svg',
                        'style/images/stickers/stickers/02/anim_52.svg',
                        'style/images/stickers/stickers/02/anim_54.svg',
                        'style/images/stickers/stickers/02/anim_55.svg',
                        'style/images/stickers/stickers/02/anim_56.svg',
                        'style/images/stickers/stickers/02/anim_57.svg',
                        'style/images/stickers/stickers/02/anim_58.svg',
                        'style/images/stickers/stickers/02/anim_59.svg',
                        'style/images/stickers/stickers/02/anim_62.svg',
                        'style/images/stickers/stickers/02/anim_63.svg',
                        'style/images/stickers/stickers/02/anim_65.svg',
                        'style/images/stickers/stickers/02/anim_67.svg',
                        'style/images/stickers/stickers/02/anim_68.svg',
                        'style/images/stickers/stickers/02/anim_69.svg',
                        'style/images/stickers/stickers/02/anim_70.svg',
                        'style/images/stickers/stickers/02/anim_72.svg',
                        'style/images/stickers/stickers/02/anim_73.svg',
                        'style/images/stickers/stickers/02/anim_74.svg',
                        'style/images/stickers/stickers/02/anim_75.svg',
                        'style/images/stickers/stickers/02/anim_76.svg',
                        'style/images/stickers/stickers/02/anim_77.svg',
                        'style/images/stickers/stickers/02/anim_78.svg',
                        'style/images/stickers/stickers/02/anim_79.svg',
                        'style/images/stickers/stickers/02/anim_80.svg',
                        'style/images/stickers/stickers/02/anim_81.svg',
                        'style/images/stickers/stickers/02/anim_82.svg',
                        'style/images/stickers/stickers/02/anim_83.svg',
                        'style/images/stickers/stickers/02/anim_84.svg',
                        'style/images/stickers/stickers/02/anim_85.svg',
                        'style/images/stickers/stickers/02/anim_86.svg',
                        'style/images/stickers/stickers/02/anim_87.svg',
                        'style/images/stickers/stickers/02/anim_88.svg',
                        'style/images/stickers/stickers/02/anim_89.svg',
                        'style/images/stickers/stickers/02/anim_90.svg',
                        'style/images/stickers/stickers/02/anim_91.svg',
                        'style/images/stickers/stickers/02/anim_92.svg',
                        'style/images/stickers/stickers/02/anim_93.svg',
                        'style/images/stickers/stickers/02/anim_94.svg',
                        'style/images/stickers/stickers/02/anim_95.svg',
                        'style/images/stickers/stickers/02/anim_96.svg',
                        'style/images/stickers/stickers/02/anim_97.svg',
                        'style/images/stickers/stickers/02/anim_98.svg',
                        'style/images/stickers/stickers/02/anim_99.svg',
                        'style/images/stickers/stickers/02/anim_100.svg',
                        'style/images/stickers/stickers/02/anim_101.svg',
                        'style/images/stickers/stickers/02/anim_102.svg',
                        'style/images/stickers/stickers/02/anim_103.svg',
                        'style/images/stickers/stickers/02/anim_104.svg'
                    ]
                },
                {
                    name: 'shapes',
                    displayImage: 'style/images/stickers/art_sticker_03_unselected.png',
                    selectedDisplayImage: 'style/images/stickers/art_sticker_03_selected.png',
                    stickers: [
                        'style/images/stickers/stickers/03/shape_01.svg',
                        'style/images/stickers/stickers/03/shape_02.svg',
                        'style/images/stickers/stickers/03/shape_03.svg',
                        'style/images/stickers/stickers/03/shape_04.svg',
                        'style/images/stickers/stickers/03/shape_05.svg',
                        'style/images/stickers/stickers/03/shape_06.svg',
                        'style/images/stickers/stickers/03/shape_07.svg',
                        'style/images/stickers/stickers/03/shape_08.svg',
                        'style/images/stickers/stickers/07/artefact_01.svg',
                        'style/images/stickers/stickers/07/artefact_02.svg',
                        'style/images/stickers/stickers/07/artefact_03.svg',
                        'style/images/stickers/stickers/07/artefact_04.svg',
                        'style/images/stickers/stickers/07/artefact_05.svg',
                        'style/images/stickers/stickers/07/artefact_06.svg',
                        'style/images/stickers/stickers/07/artefact_07.svg',
                        'style/images/stickers/stickers/07/artefact_08.svg',
                        'style/images/stickers/stickers/07/artefact_09.svg',
                        'style/images/stickers/stickers/07/artefact_10.svg',
                        'style/images/stickers/stickers/07/artefact_11.svg',
                        'style/images/stickers/stickers/07/artefact_12.svg',
                        'style/images/stickers/stickers/07/artefact_13.svg',
                        'style/images/stickers/stickers/07/fairy_01.svg',
                        'style/images/stickers/stickers/07/fairy_02.svg',
                        'style/images/stickers/stickers/07/fairy_03.svg',
                        'style/images/stickers/stickers/07/fairy_04.svg',
                        'style/images/stickers/stickers/07/fairy_05.svg',
                        'style/images/stickers/stickers/07/fairy_06.svg',
                        'style/images/stickers/stickers/07/fairy_07.svg',
                        'style/images/stickers/stickers/07/fairy_08.svg',
                        'style/images/stickers/stickers/07/fairy_09.svg',
                        'style/images/stickers/stickers/07/fairy_10.svg',
                        'style/images/stickers/stickers/07/fairy_11.svg',
                        'style/images/stickers/stickers/07/fairy_12.svg',
                        'style/images/stickers/stickers/07/fairy_13.svg',
                        'style/images/stickers/stickers/07/fairy_14.svg',
                        'style/images/stickers/stickers/07/pirate_01.svg',
                        'style/images/stickers/stickers/07/pirate_02.svg',
                        'style/images/stickers/stickers/07/pirate_03.svg',
                        'style/images/stickers/stickers/07/pirate_04.svg',
                        'style/images/stickers/stickers/07/pirate_05.svg',
                        'style/images/stickers/stickers/07/pirate_06.svg',
                        'style/images/stickers/stickers/07/pirate_07.svg',
                        'style/images/stickers/stickers/07/pirate_08.svg',
                        'style/images/stickers/stickers/07/pirate_09.svg'
                    ]
                },
                {
                    name: 'food',
                    displayImage: 'style/images/stickers/art_sticker_04_unselected.png',
                    selectedDisplayImage: 'style/images/stickers/art_sticker_04_selected.png',
                    stickers: [
                        'style/images/stickers/stickers/04/food_01.svg',
                        'style/images/stickers/stickers/04/food_02.svg',
                        'style/images/stickers/stickers/04/food_03.svg',
                        'style/images/stickers/stickers/04/food_04.svg',
                        'style/images/stickers/stickers/04/food_05.svg',
                        'style/images/stickers/stickers/04/food_06.svg',
                        'style/images/stickers/stickers/04/food_07.svg',
                        'style/images/stickers/stickers/04/food_08.svg'
                    ]
                },
                {
                    name: 'text',
                    displayImage: 'style/images/stickers/art_sticker_05_unselected.png',
                    selectedDisplayImage: 'style/images/stickers/art_sticker_05_selected.png',
                    stickers: [
                        'style/images/stickers/stickers/05/text_01.svg',
                        'style/images/stickers/stickers/05/text_02.svg',
                        'style/images/stickers/stickers/05/text_03.svg',
                        'style/images/stickers/stickers/05/text_04.svg',
                        'style/images/stickers/stickers/05/text_05.svg',
                        'style/images/stickers/stickers/05/text_06.svg',
                        'style/images/stickers/stickers/05/text_07.svg',
                        'style/images/stickers/stickers/05/text_08.svg',
                        'style/images/stickers/stickers/05/text_09.svg',
                        'style/images/stickers/stickers/05/text_10.svg',
                        'style/images/stickers/stickers/05/text_11.svg',
                        'style/images/stickers/stickers/05/text_12.svg',
                        'style/images/stickers/stickers/05/text_13.svg',
                        'style/images/stickers/stickers/05/text_14.svg',
                        'style/images/stickers/stickers/05/text_15.svg',
                        'style/images/stickers/stickers/05/text_16.svg',
                        'style/images/stickers/stickers/05/text_17.svg',
                        'style/images/stickers/stickers/05/text_18.svg',
                        'style/images/stickers/stickers/05/text_19.svg',
                        'style/images/stickers/stickers/05/text_20.svg',
                        'style/images/stickers/stickers/05/text_21.svg',
                        'style/images/stickers/stickers/05/text_22.svg',
                        'style/images/stickers/stickers/05/text_23.svg',
                        'style/images/stickers/stickers/05/text_24.svg'
                    ]
                },
                {
                    name: 'nature',
                    displayImage: 'style/images/stickers/art_sticker_06_unselected.png',
                    selectedDisplayImage: 'style/images/stickers/art_sticker_06_selected.png',
                    stickers: [
                        'style/images/stickers/stickers/06/nature_01.svg',
                        'style/images/stickers/stickers/06/nature_02.svg',
                        'style/images/stickers/stickers/06/nature_03.svg',
                        'style/images/stickers/stickers/06/nature_04.svg',
                        'style/images/stickers/stickers/06/nature_05.svg',
                        'style/images/stickers/stickers/06/nature_06.svg',
                        'style/images/stickers/stickers/06/nature_07.svg',
                        'style/images/stickers/stickers/06/nature_08.svg'
                    ]
                }
            ],
            'patterns': [
                {
                    name: 'BlueGlitter',
                    url: 'style/images/artIcons/patterns/Blue-Glitter.jpg'
                },
                {
                    name: 'GoldGlitter',
                    url: 'style/images/artIcons/patterns/Gold-Glitter.jpg'
                },
                {
                    name: 'GreenGlitter',
                    url: 'style/images/artIcons/patterns/Green-Glitter.jpg'
                },
                {
                    name: 'PinkGlitter',
                    url: 'style/images/artIcons/patterns/Pink-Glitter.jpg'
                },
                {
                    name: 'RedGlitter',
                    url: 'style/images/artIcons/patterns/Red-Glitter.jpg'
                },
                {
                    name: 'SilverGlitter',
                    url: 'style/images/artIcons/patterns/Silver-Glitter.jpg'
                },
                {
                    name: 'PurpleGlitter',
                    url: 'style/images/artIcons/patterns/Purple-Glitter.jpg'
                },
                {
                    name: 'TurquoiseGlitter',
                    url: 'style/images/artIcons/patterns/Turquoise-Glitter.jpg'
                }
            ]
        };
    }());
});