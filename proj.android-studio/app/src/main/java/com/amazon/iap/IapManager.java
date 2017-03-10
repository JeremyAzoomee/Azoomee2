package com.amazon.iap;

import java.util.List;
import java.util.Map;
import java.util.Set;

import android.util.Log;

import com.amazon.device.iap.PurchasingListener;
import com.amazon.device.iap.PurchasingService;
import com.amazon.device.iap.model.FulfillmentResult;
import com.amazon.device.iap.model.Product;
import com.amazon.device.iap.model.Receipt;
import com.amazon.device.iap.model.UserData;

import org.cocos2dx.cpp.AppActivity;

public class IapManager {
    private static final String TAG = "IAPManager";
    private final AppActivity mainActivity;
    private final SubscriptionDataSource dataSource;

    private boolean magazineSubsAvailable;
    private UserIapData userIapData;

    public IapManager(final AppActivity mainActivity) {
        this.mainActivity = mainActivity;
        this.dataSource = new SubscriptionDataSource(mainActivity.getApplicationContext());
    }

    public void setAmazonUserId(final String newAmazonUserId, final String newAmazonMarketplace) {
        // Reload everything if the Amazon user has changed.
        if (newAmazonUserId == null) {
            // A null user id typically means there is no registered Amazon
            // account.
            if (userIapData != null) {
                userIapData = null;
                refreshMagazineSubsAvailability();
            }
        } else if (userIapData == null || !newAmazonUserId.equals(userIapData.getAmazonUserId())) {
            // If there was no existing Amazon user then either no customer was
            // previously registered or the application has just started.

            // If the user id does not match then another Amazon user has
            // registered.
            userIapData = new UserIapData(newAmazonUserId, newAmazonMarketplace);
            refreshMagazineSubsAvailability();
        }
    }

    public void enablePurchaseForSkus(final Map<String, Product> productData) {
        if (productData.containsKey(MySku.MY_MAGAZINE_SUBS.getSku())) {
            magazineSubsAvailable = true;
        }
    }

    public void disablePurchaseForSkus(final Set<String> unavailableSkus) {
        if (unavailableSkus.contains(MySku.MY_MAGAZINE_SUBS.toString())) {
            magazineSubsAvailable = false;
            // reasons for product not available can be:
            // * Item not available for this country
            // * Item pulled off from Appstore by developer
            // * Item pulled off from Appstore by Amazon
            Log.d("iap", "the magazine subscription product isn't available now! ");
        }
    }

    public void handleSubscriptionPurchase(final Receipt receipt, final UserData userData) {
        try {
            if (receipt.isCanceled()) {
                // Check whether this receipt is for an expired or canceled
                // subscription
                revokeSubscription(receipt, userData.getUserId());
            } else {
                // We strongly recommend that you verify the receipt on
                // server-side.
                if (!verifyReceiptFromYourService(receipt.getReceiptId(), userData)) {
                    // if the purchase cannot be verified,
                    // show relevant error message to the customer.
                    Log.d("iap", "Purchase cannot be verified, please retry later.");
                    return;
                }
                //grantSubscriptionPurchase(receipt, userData);
            }
            return;
        } catch (final Throwable e) {
            Log.d("iap", "Purchase cannot be completed, please retry");
        }

    }

    private void grantSubscriptionPurchase(final Receipt receipt, final UserData userData) {

        final MySku mySku = MySku.fromSku(receipt.getSku(), userIapData.getAmazonMarketplace());
        // Verify that the SKU is still applicable.
        if (mySku != MySku.MY_MAGAZINE_SUBS) {
            Log.w(TAG, "The SKU [" + receipt.getSku() + "] in the receipt is not valid anymore ");
            // if the sku is not applicable anymore, call
            // PurchasingService.notifyFulfillment with status "UNAVAILABLE"
            PurchasingService.notifyFulfillment(receipt.getReceiptId(), FulfillmentResult.UNAVAILABLE);
            return;
        }
        try {
            // Set the purchase status to fulfilled for your application
            saveSubscriptionRecord(receipt, userData.getUserId());
            PurchasingService.notifyFulfillment(receipt.getReceiptId(), FulfillmentResult.FULFILLED);

        } catch (final Throwable e) {
            // If for any reason the app is not able to fulfill the purchase,
            // add your own error handling code here.
            Log.e(TAG, "Failed to grant entitlement purchase, with error " + e.getMessage());
        }

    }

    public void handleReceipt(final String requestId, final Receipt receipt, final UserData userData) {
        switch (receipt.getProductType()) {
        case CONSUMABLE:
            // check consumable sample for how to handle consumable purchases
            break;
        case ENTITLED:
            // check entitlement sample for how to handle consumable purchases
            break;
        case SUBSCRIPTION:
            handleSubscriptionPurchase(receipt, userData);
            break;
        }
    }

    public void purchaseFailed(final String sku) {
        Log.d("iap", "Purchase failed!");
    }

    public UserIapData getUserIapData() {
        return this.userIapData;
    }

    public boolean isMagazineSubsAvailable() {
        return magazineSubsAvailable;
    }

    public void setMagazineSubsAvailable(final boolean magazineSubsAvailable) {
        this.magazineSubsAvailable = magazineSubsAvailable;
    }

    public void disableAllPurchases() {
        this.setMagazineSubsAvailable(false);
        refreshMagazineSubsAvailability();
    }

    public void refreshMagazineSubsAvailability() {
        final boolean available = magazineSubsAvailable && userIapData!=null;
        //mainActivity.setMagazineSubsAvail(available,
                                          //userIapData != null && !userIapData.isSubsActiveCurrently();
    }

    public void deactivate() {
        dataSource.close();

    }

    public void activate() {
        dataSource.open();

    }

    public void reloadSubscriptionStatus() {
        final List<SubscriptionRecord> subsRecords = dataSource.getSubscriptionRecords(userIapData.getAmazonUserId());
        userIapData.setSubscriptionRecords(subsRecords);
        userIapData.reloadSubscriptionStatus();
        refreshMagazineSubsAvailability(); 
    }

    private void saveSubscriptionRecord(final Receipt receipt, final String userId) {
        // TODO replace with your own implementation

        dataSource
            .insertOrUpdateSubscriptionRecord(receipt.getReceiptId(),
                                              userId,
                                              receipt.getPurchaseDate().getTime(),
                                              receipt.getCancelDate() == null ? SubscriptionRecord.TO_DATE_NOT_SET
                                                      : receipt.getCancelDate().getTime(),
                                              receipt.getSku());

    }

    private boolean verifyReceiptFromYourService(final String receiptId, final UserData userData) {
        // TODO Add your own server side accessing and verification code
        return true;
    }

    private void revokeSubscription(final Receipt receipt, final String userId) {
        final String receiptId = receipt.getReceiptId();
        dataSource.cancelSubscription(receiptId, receipt.getCancelDate().getTime());

    }

}
