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
            if(data == null) {
                data = new Dictionary<string, string>();
            }
            if (data.Count < 1) return "";
            string result = ",";
            foreach (string key in data.Keys) result += "\"" + key + "\":\"" + data[key] + "\",";
            return result.Substring(0, result.Length - 2);
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