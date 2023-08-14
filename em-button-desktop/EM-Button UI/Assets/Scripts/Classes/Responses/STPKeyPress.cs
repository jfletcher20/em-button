using System.Collections.Generic;

/// <summary>
/// Response when it pertains to device status (all OLED Monitor data and more).
/// </summary>
[System.Serializable]
public class STPKeyPressResponseModel : STPResponseModel {
    public new STPKeyPressDataModel data;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("status", status);
        result.Add("data", data.toDictionary());
        return result;
    }
}

[System.Serializable]
public class STPKeyPressDataModel : STPDataModel {
    public int keyId, actionId;
    public int times_pressed;
    public float time_when_pressed;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("message", message);
        result.Add("keyId", keyId);
        result.Add("actionId", actionId);
        result.Add("times_pressed", times_pressed);
        result.Add("time_when_pressed", time_when_pressed);
        return result;
    }
    public enum Keys {
        keyId, actionId, activation_point, times_pressed, time_when_pressed
    }
}