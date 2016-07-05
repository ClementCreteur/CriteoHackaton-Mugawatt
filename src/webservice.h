#include "HttpClient/HttpClient.h"

class WebService
{
    HttpClient http;
    http_request_t request;
    http_request_t twitterRequest;
    http_response_t response;
    String drinkPath;
    String finishPath;
    String batteryPath;

public:
    WebService();
    void sendDrink();
    void sendDrinkIsCold();
    void sendBattery(float battery);
};