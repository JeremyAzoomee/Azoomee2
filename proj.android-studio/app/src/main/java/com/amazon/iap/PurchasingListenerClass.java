package com.amazon.iap;

import java.util.HashSet;
import java.util.Set;

import android.util.Log;

import com.amazon.device.iap.PurchasingListener;
import com.amazon.device.iap.PurchasingService;
import com.amazon.device.iap.model.ProductDataResponse;
import com.amazon.device.iap.model.PurchaseResponse;
import com.amazon.device.iap.model.PurchaseUpdatesResponse;
import com.amazon.device.iap.model.Receipt;
import com.amazon.device.iap.model.UserDataResponse;

import org.cocos2dx.cpp.AppActivity;

public class PurchasingListenerClass implements PurchasingListener {

    private final IapManager iapManager;
    private AppActivity appActivity;

    public void setMainActivity(AppActivity sentAppActivity)
    {
        appActivity = sentAppActivity;
    }

    public PurchasingListenerClass(final IapManager iapManager) {
        Log.d("IAPAPI", "Listener starts up!");
        this.iapManager = iapManager;
    }

    @Override
    public void onUserDataResponse(final UserDataResponse response) {
        Log.d("IAPAPIListener", "onGetUserDataResponse: requestId (" + response.getRequestId()
                   + ") userIdRequestStatus: "
                   + response.getRequestStatus()
                   + ")");

        final UserDataResponse.RequestStatus status = response.getRequestStatus();
        switch (status) {
        case SUCCESSFUL:
            Log.d("IAPAPIListener", "onUserDataResponse: get user id (" + response.getUserData().getUserId()
                       + ", marketplace ("
                       + response.getUserData().getMarketplace()
                       + ") ");
            iapManager.setAmazonUserId(response.getUserData().getUserId(), response.getUserData().getMarketplace());
            break;

        case FAILED:
        case NOT_SUPPORTED:
            Log.d("IAPAPIListener", "onUserDataResponse failed, status code is " + status);
            iapManager.setAmazonUserId(null, null);
            break;
        }
    }

    @Override
    public void onProductDataResponse(final ProductDataResponse response) {
        final ProductDataResponse.RequestStatus status = response.getRequestStatus();
        Log.d("IAPAPIListener", "onProductDataResponse: RequestStatus (" + status + ")");

        switch (status) {
        case SUCCESSFUL:
            Log.d("IAPAPIListener", "onProductDataResponse: successful.  The item data map in this response includes the valid SKUs");
            final Set<String> unavailableSkus = response.getUnavailableSkus();
            Log.d("IAPAPI", "onProductDataResponse: " + unavailableSkus.size() + " unavailable skus");
            iapManager.enablePurchaseForSkus(response.getProductData());
            iapManager.disablePurchaseForSkus(response.getUnavailableSkus());
            iapManager.refreshMagazineSubsAvailability();

            break;
        case FAILED:
        case NOT_SUPPORTED:
            Log.d("IAPAPIListener", "onProductDataResponse: failed, should retry request");
            iapManager.disableAllPurchases();
            break;
        }
    }

    @Override
    public void onPurchaseUpdatesResponse(final PurchaseUpdatesResponse response) {
        Log.d("IAPAPIListener", "onPurchaseUpdatesResponse: requestId (" + response.getRequestId()
                   + ") purchaseUpdatesResponseStatus ("
                   + response.getRequestStatus()
                   + ") userId ("
                   + response.getUserData().getUserId()
                   + ")");
        final PurchaseUpdatesResponse.RequestStatus status = response.getRequestStatus();
        switch (status) {
        case SUCCESSFUL:
            iapManager.setAmazonUserId(response.getUserData().getUserId(), response.getUserData().getMarketplace());
            for (final Receipt receipt : response.getReceipts()) {
                iapManager.handleReceipt(response.getRequestId().toString(), receipt, response.getUserData());
            }
            if (response.hasMore()) {
                PurchasingService.getPurchaseUpdates(false);
            }
            iapManager.reloadSubscriptionStatus();
            break;
        case FAILED:
        case NOT_SUPPORTED:
            Log.d("IAPAPIListener", "onProductDataResponse: failed, should retry request");
            iapManager.disableAllPurchases();
            break;
        }

    }

    @Override
    public void onPurchaseResponse(final PurchaseResponse response) {
        final String requestId = response.getRequestId().toString();
        final String userId = response.getUserData().getUserId();
        final PurchaseResponse.RequestStatus status = response.getRequestStatus();
        Log.d("IAPAPIListener", "onPurchaseResponse: requestId (" + requestId
                   + ") userId ("
                   + userId
                   + ") purchaseRequestStatus ("
                   + status
                   + ")");

        appActivity.setAmazonUserid(userId);

        switch (status) {
        case SUCCESSFUL:
            final Receipt receipt = response.getReceipt();
            Log.d("IAPAPIListener", "onPurchaseResponse: receipt json:" + receipt.toJSON());

            appActivity.setReceiptId(receipt.getReceiptId());
            appActivity.setRequestId(response.getRequestId().toString());

            Log.d("appactivity: ", appActivity.receiptId);
            Log.d("About to send data", "To appActivity");
            appActivity.sendCollectedDataToCocos();

            iapManager.handleReceipt(response.getRequestId().toString(), receipt, response.getUserData());
            iapManager.reloadSubscriptionStatus();

            break;
        case ALREADY_PURCHASED:
            Log.i("IAPAPIListener",
                  "onPurchaseResponse: already purchased, you should verify the subscription purchase on your side and make sure the purchase was granted to customer");
            break;
        case INVALID_SKU:
            Log.d("IAPAPIListener",
                  "onPurchaseResponse: invalid SKU!  onProductDataResponse should have disabled buy button already.");
            final Set<String> unavailableSkus = new HashSet<String>();
            unavailableSkus.add(response.getReceipt().getSku());
            iapManager.disablePurchaseForSkus(unavailableSkus);
            break;
        case FAILED:
        case NOT_SUPPORTED:
            Log.d("IAPAPIListener", "onPurchaseResponse: failed so remove purchase request from local storage");
            iapManager.purchaseFailed(response.getReceipt().getSku());
            break;
        }

    }

}
