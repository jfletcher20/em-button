using System.Collections.Generic;

public enum STPMethod { GET, POST, PUT, DELETE };

public struct STPCommand {
    public string route;
    public STPMethod method;
    public Dictionary<string, string> data;

    public override string ToString() => prefix + "{" + apiRoute + "," + apiMethod + apiData + "}";
    
    private const string prefix = "STP1.0";
    private string apiRoute { get { return "\"route\":\"" + route + "\""; } }
    private string apiMethod { get { return "\"method\":\"" + method.ToString() + "\""; } }
    private string apiData {
        get {
            if (data == null) {
                data = new Dictionary<string, string>();
            }
            if (data.Count < 1) return "";
            string result = ",";
            foreach (string key in data.Keys) {
                string value = data[key];
                if (int.TryParse(value, out _)) {
                    result += "\"" + key + "\":" + value + ",";
                } else if (double.TryParse(value, out _)) {
                    result += "\"" + key + "\":" + value + ",";
                } else if (bool.TryParse(value, out _)) {
                    result += "\"" + key + "\":" + value.ToLower() + ",";
                } else {
                    result += "\"" + key + "\":\"" + value + "\",";
                    if (value.StartsWith("[{")) result = result.Replace("\"[{", "[{");
                    if (value.EndsWith("}]")) result = result.Replace("}]\"", "}]");
                }
            }
            return result.Substring(0, result.Length - 1);
        }
    }


    public static STPCommand from(string route, STPMethod method) {
        STPCommand command = new STPCommand();
        command.route = route;
        command.method = method;
        return command;
    }

    public static STPCommand from(string route, STPMethod method, Dictionary<string, string> data) {
        STPCommand command = new STPCommand();
        command.route = route;
        command.method = method;
        command.data = data;
        return command;
    }

}