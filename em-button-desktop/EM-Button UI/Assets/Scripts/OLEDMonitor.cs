using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OLEDMonitor : MonoBehaviour {
    public SerialMonitor serialMonitor;
    private float lastUpdate = 0;
    void Update() {
        if(serialMonitor.latest != "" && serialMonitor.read().Key != lastUpdate) {
            lastUpdate = serialMonitor.latestUpdate;
            //serialMonitor.refreshMonitor(specifyData());
            serialMonitor.updateMonitor(specifyData());
        }
    }

    Dictionary<string, object> empty = new Dictionary<string, object>();
    private Dictionary<string, object> specifyData() {
        string latest = serialMonitor.latest;
        if (latest.Contains("current_state")) { 
            STPDeviceStatusResponseModel status = statusFromJson(latest);
            Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
            dataToDisplay.Add("Device is on", status.data.current_state.device_enabled);
            dataToDisplay.Add("Bluetooth connected", status.data.current_state.ble_connected);
            dataToDisplay.Add("Current", status.data.current_state.current_value + " (" + status.data.current_state.value_normalized + ")");
            KeyValuePair<string, object> kvp = pressedData(status.data.current_state.button_pressed, status.data.current_state.times_pressed);
            dataToDisplay.Add(kvp.Key, kvp.Value);
            return dataToDisplay;
        } else if(latest.Contains("time_when_pressed")) {
            STPKeyPressResponseModel keyPressed = keyPressFromJson(latest);
            Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
            KeyValuePair<string, object> kvp = pressedData(keyPressed.data.actionId, keyPressed.data.times_pressed);
            dataToDisplay.Add(kvp.Key, kvp.Value);
            return dataToDisplay;
        } else {
            return empty;
        }
    }

    KeyValuePair<string, object> pressedData(int button_pressed, int times_pressed) {
        int val = -1;
        try { val = button_pressed; } catch { }
        object value = (val != -1 ? val : "__") + " (x" + times_pressed + ")";
        return new KeyValuePair<string, object>("Pressed", value);
    }

    public Dictionary<string, object> getDictionaryFromJson(string json) {
        if (json.Contains("current_state")) {
            return statusFromJson(json).toDictionary();
        } else if(json.Contains("time_when_pressed")) {
            return keyPressFromJson(json).toDictionary();
        } else {
            return JsonUtility.FromJson<STPResponseModel>(json).toDictionary();
        }
    }

    private STPDeviceStatusResponseModel statusFromJson(string json) => JsonUtility.FromJson<STPDeviceStatusResponseModel>(json);
    private STPKeyPressResponseModel keyPressFromJson(string json) => JsonUtility.FromJson<STPKeyPressResponseModel>(json);
}
