using System.Collections.Generic;

[System.Serializable]
public class EmbAction {
    public int actionId, keyId;
    public double activation_point;
    public Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object>();
        result.Add("actionId", actionId);
        result.Add("keyId", keyId);
        result.Add("activation_point", activation_point);
        return result;
    }
}