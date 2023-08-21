using System.Collections.Generic;

[System.Serializable]
public class EmbButton {
    public int id = 0;
    public int electromagnet = 13, hall_sensor = 4;
    public double electromagnet_power = 1.0;
    public List<EmbAction> actions = new List<EmbAction>(3);

    public Dictionary<string, object> toDictionary() {
        Dictionary<string, object> result = new Dictionary<string, object> {
            { "id", id },
            { "electromagnet", electromagnet },
            { "hall_sensor", hall_sensor },
            { "electromagnet_power", electromagnet_power },
            { "actions", new List<Dictionary<string, object>>() }
        };

        foreach (var action in actions) {
            Dictionary<string, object> actions = new Dictionary<string, object> {
                { "actionId", action.actionId },
                { "keyId", action.keyId },
                { "activation_point", action.activation_point }
            };
            ((dynamic)result["actions"]).Add(actions);
        }

        return result;
    }

    public Dictionary<string, string> toStringDictionary() {

        Dictionary<string, string> result = new Dictionary<string, string> {
            { "id", id.ToString() },
            { "electromagnet", electromagnet.ToString() },
            { "hall_sensor", hall_sensor.ToString() },
            { "electromagnet_power", electromagnet_power.ToString() },
            { "actions", "[" },
        };

        foreach (EmbAction action in actions) {
            result["actions"] += "{\"actionId\":" + action.actionId + ","
                + "\"keyId\":" + action.keyId.ToString() + ","
                + "\"activation_point\":" + action.activation_point.ToString() + "},";
        }

        if(result["actions"].Length > 1)
            result["actions"] = result["actions"].Substring(0, result["actions"].Length - 1);
        
        result["actions"] += "]";

        return result;
    }

    public static EmbButton from(DeviceStatusResponseModel status) {
        return new EmbButton {
            id = status.data.current_value.id,
            actions = status.data.current_value.actions,
            electromagnet = status.data.current_value.electromagnet,
            hall_sensor = status.data.current_value.hall_sensor,
            electromagnet_power = status.data.current_value.electromagnet_power,
        };
    }

    public void update(DeviceStatusResponseModel status) {
        id = status.data.current_value.id;
        actions = status.data.current_value.actions;
        electromagnet = status.data.current_value.electromagnet;
        hall_sensor = status.data.current_value.hall_sensor;
        electromagnet_power = status.data.current_value.electromagnet_power;
    }

    public string toJson() {
        Dictionary<string, object> dictionary = toDictionary();
        return ToJsonString(dictionary);
    }

    private string ToJsonString(Dictionary<string, object> dictionary) {
        List<string> jsonItems = new List<string>();

        foreach (var kvp in dictionary) {
            string key = kvp.Key;
            object value = kvp.Value;

            if (value is List<dynamic> list) {
                List<string> listItems = new List<string>();
                foreach (var listItem in list) {
                    if (listItem is Dictionary<string, object> listItemDict) {
                        listItems.Add(ToJsonString(listItemDict));
                    } else {
                        listItems.Add(JsonValue(listItem));
                    }
                }
                jsonItems.Add($"\"{key}\":[{string.Join(",", listItems)}]");
            } else if (value is Dictionary<string, object> dict) {
                jsonItems.Add($"\"{key}\":{ToJsonString(dict)}");
            } else {
                jsonItems.Add($"\"{key}\":{JsonValue(value)}");
            }
        }

        return $"{{{string.Join(",", jsonItems)}}}";
    }

    private string JsonValue(object value) {
        if (value is string) {
            return $"\"{value}\"";
        } else {
            return value.ToString();
        }
    }
}