using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CalibrationButton : MonoBehaviour {
    public SerialMonitor serialMonitor;
    private void Awake() {
        GetComponent<Button>().onClick.AddListener(callCalibrationRoute);
    }
    private void callCalibrationRoute() {
        serialMonitor.sendCommand(new STPCommand { method = STPMethod.GET, route = "/device/calibrate/" });
    }
}
