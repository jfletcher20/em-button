enum Routes {
    ROOT,
    DB,
    ROUTES,
    CALIBRATE,
    ENABLE,
    DISABLE,
    DATA,
    SAVE,
    ELECTROMAGNET,
    ELECTROMAGNET_POWER,
    HALLSENSOR,
    HALLSENSOR_NORMALIZED,
};

const String routes[] = {
    "/",
    "/db/", // example: STP1.0{"route":"/db/","method":"get"}
    "/device/routes/",
    "/device/calibrate/",
    "/device/enable/",
    "/device/disable/",
    "/device/data/",
    "/device/save",
    "/device/electromagnet/",
    "/device/electromagnet/power/",
    "/device/hallsensor/",
    "/device/hallsensor/normalized/",
};