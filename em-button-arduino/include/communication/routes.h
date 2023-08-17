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
    "/", // GET
    "/db/", // GET, POST, PUT, DELETE
    "/device/routes/", // GET
    "/device/calibrate/", // GET
    "/device/enable/", // PUT
    "/device/disable/", // PUT
    "/device/data/", // GET
    "/device/save/", // PUT
    "/device/electromagnet/", // GET
    "/device/electromagnet/power/", // PUT
    "/device/hallsensor/", // GET
    "/device/hallsensor/normalized/", // GET
}; // example: STP1.0{"route":"/db/","method":"get"}