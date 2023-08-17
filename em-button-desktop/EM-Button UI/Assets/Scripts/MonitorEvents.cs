using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonitorEvents : MonoBehaviour {

    public EmbButton embData;

    private SerialMonitor serialMonitor;
    private float lastUpdate = 0;
    private void Awake() {
        serialMonitor = GetComponent<SerialMonitor>();
    }

    void Update() {
        if (serialMonitor.latest != "" && serialMonitor.read().Key != lastUpdate) {
            lastUpdate = serialMonitor.latestUpdate;
            serialMonitor.updateMonitor(specifyData());
        }
    }

    Dictionary<string, object> empty = new Dictionary<string, object>();
    private Dictionary<string, object> specifyData() {
        string latest = serialMonitor.latest;
        if (latest.Contains("current_state")) {
            return statusUpdate(latest);
        } else if (latest.Contains("time_when_pressed")) {
            return keyPressUpdate(latest);
        } else if (latest.Contains("ble_connected") && latest.Contains("\"status\":300")) {
            return bleConnectionUpdate(latest);
        } else if (latest.Contains("enabled") && latest.Contains("\"status\":204")) {
            return enabledUpdate(latest);
        } else if (latest.Contains("routes")) {
            return routesUpdate(latest);
        } else {
            Debug.LogWarning("DID NOT PARSE: " + latest);
            return empty;
        }
    }

    Dictionary<string, object> statusUpdate(string latest) {
        var deviceStatus = statusFromJson(latest);
        if (latest.Contains("\"ble_connected\":true")) deviceStatus.data.current_state.ble_connected = true;
        Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
        dataToDisplay.Add("Device is on", deviceStatus.data.current_state.device_enabled);
        dataToDisplay.Add("Bluetooth connected", deviceStatus.data.current_state.ble_connected);
        dataToDisplay.Add("Current", deviceStatus.data.current_state.current_value + " (" + deviceStatus.data.current_state.value_normalized + ")");
        KeyValuePair<string, object> kvp = pressedData(deviceStatus.data.current_state.button_pressed, deviceStatus.data.current_state.times_pressed);
        dataToDisplay.Add(kvp.Key, kvp.Value);
        if (embData == null) embData = EmbButton.from(deviceStatus);
        else embData.update(deviceStatus);
        return dataToDisplay;
    }

    public Dictionary<string, object> keyPressUpdate(string latest) {
        KeyPressResponseModel keyPressed = keyPressFromJson(latest);
        Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
        KeyValuePair<string, object> kvp = pressedData(keyPressed.data.actionId, keyPressed.data.times_pressed);
        dataToDisplay.Add(kvp.Key, kvp.Value);
        return dataToDisplay;
    }

    public Dictionary<string, object> bleConnectionUpdate(string latest) {
        STPBleConnectionResponseModel bleConnected = bleConnectionFromJson(latest);
        if (latest.Contains("\"ble_connected\":true")) bleConnected.data.ble_connected = true;
        Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
        dataToDisplay.Add("Bluetooth connected", bleConnected.data.ble_connected);
        return dataToDisplay;
    }

    public Dictionary<string, object> enabledUpdate(string latest) {
        EnabledResponseModel deviceEnabled = enabledFromJson(latest);
        Dictionary<string, object> dataToDisplay = new Dictionary<string, object>();
        dataToDisplay.Add("Device is on", deviceEnabled.data.enabled);
        return dataToDisplay;
    }

    /// <summary>
    /// Updates local variable instead of updating routes on monitor.
    /// </summary>
    /// <param name="latest">Latest API response.</param>
    /// <returns>Empty, since routes are no longer being loaded from the API.</returns>
    public Dictionary<string, object> routesUpdate(string latest) {
        return empty;
    }

    KeyValuePair<string, object> pressedData(int button_pressed, int times_pressed) {
        int val = -1;
        try { val = button_pressed; } catch { }
        object value = (val != -1 ? val : "-") + " (x" + times_pressed + ")";
        return new KeyValuePair<string, object>("Pressed", value);
    }

    public Dictionary<string, object> getDictionaryFromJson(string json) {
        if (json.Contains("current_state")) {
            return statusFromJson(json).toDictionary();
        } else if (json.Contains("time_when_pressed")) {
            return keyPressFromJson(json).toDictionary();
        } else if (json.Contains("ble_connected") && json.Contains("\"status\":300")) {
            return bleConnectionFromJson(json).toDictionary();
        } else {
            return JsonUtility.FromJson<STPResponseModel>(json).toDictionary();
        }
    }

    private DeviceStatusResponseModel statusFromJson(string json) => JsonUtility.FromJson<DeviceStatusResponseModel>(json);
    private KeyPressResponseModel keyPressFromJson(string json) => JsonUtility.FromJson<KeyPressResponseModel>(json);
    private STPBleConnectionResponseModel bleConnectionFromJson(string json) => JsonUtility.FromJson<STPBleConnectionResponseModel>(json);
    private EnabledResponseModel enabledFromJson(string json) => JsonUtility.FromJson<EnabledResponseModel>(json);
    private RoutesResponseModel routesFromJson(string json) => JsonUtility.FromJson<RoutesResponseModel>(json);
}
