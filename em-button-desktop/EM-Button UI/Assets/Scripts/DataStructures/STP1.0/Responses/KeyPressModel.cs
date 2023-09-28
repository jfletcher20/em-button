using System.Collections.Generic;

/// <summary>
/// Response when it pertains to a keypress update.
/// </summary>
[System.Serializable]
public class KeyPressResponseModel : STPResponseModel {
    public new KeyPressDataModel data;
    public override Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("status", status);
        result.Add("data", data.toDictionary());
        return result;
    }
}

[System.Serializable]
public class KeyPressDataModel : STPDataModel {
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