using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using System.Linq;

public class SettingsTab : MonoBehaviour {
    public SerialMonitor serialMonitor;

    public Button settingsButton;
    public GameObject settingsMenu;
    private Animator settingsMenuAnimator;

    public GameObject routeNavigator;
    public GameObject routeSelectablePrefab;

    public MouseInteractions embModelClickInteractions;

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
        embModelClickInteractions.disabled = !embModelClickInteractions.disabled;
    }

    private bool _initRoutes = true;
    public void initRoutes() {
        if (!_initRoutes) return;
        else _initRoutes = false;
        clearChildren();
        List<string> displayed = new List<string>();
        foreach (RouteManagement.STPRouteDetails route in RouteManagement.routesList) {
            if (displayed.Contains(route.displayText)) continue;
            displayed.Add(route.displayText);
            Selectable button = Instantiate(routeSelectablePrefab, routeNavigator.transform).GetComponent<Selectable>();
            TextMeshProUGUI text = button.GetComponentInChildren<TextMeshProUGUI>();
            text.text = route.displayText;
            text.color = route.displayColor;
        }
        GetComponent<SelectedRouteManager>().Initialize(routeNavigator.GetComponentsInChildren<Selectable>().ToList());
    }

    private void _loadRoute(RouteManagement.STPRouteDetails route) {
        STPCommand command = new STPCommand { route = route.path, method = route.method };
        if (route.validate(command))
            serialMonitor.sendCommand(command);
    }

    public void clearChildren() {
        foreach (Transform child in routeNavigator.transform) {
            Destroy(child.gameObject);
        }
    }

}
