#include "webservice.h"

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

WebService::WebService()
{
    request.hostname = "www.xxxxxxxxx.com";
    request.port = 80;
    twitterRequest.hostname = "www.api.twitter.com";
    twitterRequest.port = 80;
    String deviceId = Particle.deviceID();
    String cmdPath = "/mugawatt/ws.php?id=" + deviceId + "&cmd=";
    drinkPath = cmdPath + "drink";
    finishPath = cmdPath + "finish";
    batteryPath = cmdPath + "ping&battery=";
}

void WebService::sendDrink()
{
    request.path = drinkPath;
    http.get(request, response, headers);
}

void WebService::sendDrinkIsCold()
{
    request.path = finishPath;
    http.get(request, response, headers);
}

void WebService::sendBattery(float battery)
{
    request.path = batteryPath + String((int)battery);
    http.get(request, response, headers);
}