package com.amazon.iap;

import org.cocos2dx.cpp.AppActivity;

public enum MySku {

    MY_MAGAZINE_SUBS(AppActivity.getAmazonSku(), "UK");

    private final String sku;
    private final String availableMarkpetplace;

    public String getSku() {
        return this.sku;
    }

    public String getAvailableMarketplace() {
        return this.availableMarkpetplace;
    }

    private MySku(final String sku, final String availableMarkpetplace) {
        this.sku = sku;
        this.availableMarkpetplace = availableMarkpetplace;
    }

    public static MySku fromSku(final String sku, final String marketplace) {
        if (MY_MAGAZINE_SUBS.getSku().equals(sku) && (null == marketplace || MY_MAGAZINE_SUBS.getAvailableMarketplace()
                .equals(marketplace))) {
            return MY_MAGAZINE_SUBS;
        }
        return null;
    }

}
