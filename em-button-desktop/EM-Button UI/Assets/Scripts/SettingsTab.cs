using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class SettingsTab : MonoBehaviour {
    public SerialMonitor serialMonitor;

    public Button settingsButton;
    public GameObject settingsMenu;
    private Animator settingsMenuAnimator;

    public GameObject routeNavigator;
    public Button routeButtonPrefab;

    private void Awake() {
        settingsMenuAnimator = settingsMenu.GetComponent<Animator>();
        clearChildren();
    }

    void Start() {
        Button btn = settingsButton.GetComponent<Button>();
        btn.onClick.AddListener(animateSettingsMenu);
        btn.onClick.AddListener(initRoutes);
    }

    void animateSettingsMenu() {
        settingsMenuAnimator.SetBool("enabled", !settingsMenuAnimator.GetBool("enabled"));
    }

    void initRoutes() {
        STPCommand command = STPCommand.from("/device/routes/", STPMethod.GET);
        serialMonitor.sendCommand(command);
    }

    public void initRoutes(List<string> routes) {
        // Destroy existing buttons
        clearChildren();

        // Create new buttons for each route
        foreach (string route in routes) {
            Button button = Instantiate(routeButtonPrefab, routeNavigator.transform);
            TextMeshProUGUI text = button.GetComponentInChildren<TextMeshProUGUI>();
            text.text = route; // Set button text to route name
            _routeColor(text, route);
            button.onClick.AddListener(() => _loadRoute(route));
        }
    }

    private void _routeColor(TextMeshProUGUI text, string route) {
        if (route.Contains("electromagnet")) {
            text.color = Color.red;
        } else if (route.Contains("hallsensor")) {
            text.color = Color.magenta;
        } else if (route.Contains("device")) {
            text.color = Color.blue;
        } else if (route.Contains("db")) {
            text.color = Color.green;
        }
    }

    private void _loadRoute(string route) {
        STPCommand command = new STPCommand { route = route, method = STPMethod.GET };
        serialMonitor.sendCommand(command);
    }

    public void clearChildren() {
        foreach (Transform child in routeNavigator.transform) {
            Destroy(child.gameObject);
        }
    }

}
