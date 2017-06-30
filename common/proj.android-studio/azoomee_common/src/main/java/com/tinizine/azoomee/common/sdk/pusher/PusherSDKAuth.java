package com.tinizine.azoomee.common.sdk.pusher;

import android.util.Log;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;

import javax.net.ssl.HttpsURLConnection;

import com.pusher.client.AuthorizationFailureException;
import com.pusher.client.Authorizer;

/**
 * Auth a Pusher request with Azoomee server.
 */
public class PusherSDKAuth implements Authorizer
{
    private final static String TAG = "PusherSDKAuth";
    private URL _url;
    private String _method = "GET";
    private HashMap<String, String> _headers = new HashMap<String, String>();
    private String _bodyData = "";


    /// Request the C++ create an AzoomeeRequest
    /// This will trigger a call to setRequestParams from C++ side
    private native void buildSignedAzoomeeRequest(PusherSDKAuth authInstance, String channelName, String socketId);

    /// Setup the next auth request
    public void setRequestParams(final String url, final String method, final String[] headerKeys, final String[] headerValues, final String bodyData)
    {
        try
        {
            _url = new URL(url);
        }
        catch(final MalformedURLException e)
        {
            throw new IllegalArgumentException("Could not parse authentication end point into a valid URL", e);
        }
        _method = method;
        _bodyData = bodyData;

        _headers.clear();
        for(int i = 0; i < headerKeys.length; ++i)
        {
            _headers.put(headerKeys[i], headerValues[i]);
        }
    }


    @Override
    public String authorize(final String channelName, final String socketId) throws AuthorizationFailureException
    {
        Log.d(TAG, "authorize: " + channelName + ", " + socketId);
        // Build the signed Azoomee req from c++ side
        buildSignedAzoomeeRequest(this, channelName, socketId);
        Log.d(TAG, "buildSignedAzoomeeRequest done: _url=" + _url + ", _method=" + _method);

        try
        {
            HttpURLConnection connection;
            if(_url.getProtocol().equals("https"))
            {
                connection = (HttpsURLConnection)_url.openConnection();
            }
            else
            {
                connection = (HttpURLConnection)_url.openConnection();
            }
            connection.setDoOutput(false);
            connection.setDoInput(true);
            connection.setInstanceFollowRedirects(false);
            connection.setRequestMethod(_method);
            connection.setUseCaches(false);

            // Add headers
            for(final String headerName : _headers.keySet())
            {
                final String headerValue = _headers.get(headerName);
                Log.d(TAG, "Header: " + headerName + "=" + headerValue);
                connection.setRequestProperty(headerName, headerValue);
            }

            if(_method == "POST")
            {
                connection.setDoOutput(true);
                connection.setRequestProperty("Content-Length", "" + Integer.toString(_bodyData.getBytes().length));
                // Write body
                final DataOutputStream wr = new DataOutputStream(connection.getOutputStream());
                wr.writeBytes(_bodyData);
                wr.flush();
                wr.close();
            }

            Log.d(TAG, "authorize: Read response");

            // Read response
            final int responseHttpStatus = connection.getResponseCode();
            Log.d(TAG, "authorize: responseHttpStatus=" + responseHttpStatus);

            InputStream is = null;
            if(responseHttpStatus != HttpURLConnection.HTTP_OK)
            {
                is = connection.getErrorStream();
            }
            else
            {
                is = connection.getInputStream();
            }

            final BufferedReader rd = new BufferedReader(new InputStreamReader(is));
            String line;
            final StringBuffer response = new StringBuffer();
            while((line = rd.readLine()) != null)
            {
                response.append(line);
            }
            rd.close();

            Log.d(TAG, "authorize: response=" + response.toString());

            // Check response code
            if(responseHttpStatus != 200 && responseHttpStatus != 201)
            {
                throw new AuthorizationFailureException(response.toString());
            }

            return response.toString();
        }
        catch(final IOException e)
        {
            Log.e(TAG, "Auth error: " + e.toString());
            throw new AuthorizationFailureException(e);
        }
    }
}
