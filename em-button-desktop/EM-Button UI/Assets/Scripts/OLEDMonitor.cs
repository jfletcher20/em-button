using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OLEDMonitor : MonoBehaviour {
    public SerialMonitor serialMonitor;
    private float lastUpdate = 0;
    void Update() {
        if(serialMonitor.latest != "" && serialMonitor.read().Key != lastUpdate) {
            lastUpdate = serialMonitor.latestUpdate;
            serialMonitor.refreshMonitor(specifyData());
        }
    }

    private Dictionary<string, object> specifyData() {
        STPDeviceStatusResponseModel status = deviceStatusFromJson(serialMonitor.latest);
        Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
        dataToDisplay.Add("Device is on", status.data.current_state.device_enabled);
        dataToDisplay.Add("Bluetooth connected", status.data.current_state.ble_connected);
        dataToDisplay.Add("Current", status.data.current_state.current_value + " (" + status.data.current_state.value_normalized + ")");
        int val = -1;
        try { val = int.Parse(status.data.current_state.button_pressed.ToString()); } catch { }
        dataToDisplay.Add("Pressed", val != -1 ? "/\\" : "__" + " (x" + status.data.current_state.times_pressed);
        return dataToDisplay;
    }

    public Dictionary<string, object> getDictionaryFromJson(string json) {
        if(json.Contains("current_state")) {
            return deviceStatusFromJson(json).toDictionary();
        } else {
            return JsonUtility.FromJson<STPResponseModel>(json).toDictionary();
        }
    }

    private STPDeviceStatusResponseModel deviceStatusFromJson(string json) => JsonUtility.FromJson<STPDeviceStatusResponseModel>(json);
}
