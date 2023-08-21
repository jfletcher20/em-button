using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RestartButton : MonoBehaviour {
    public SerialMonitor serialMonitor;
    public Button settingsButton, apiButton;
    private void Awake() {
        GetComponent<Button>().onClick.AddListener(callRestartRoute);
    }
    private void callRestartRoute() {
        serialMonitor.sendCommand(new STPCommand { method = STPMethod.GET, route = "/" });
        if(!settingsButton.enabled && !apiButton.enabled ) {
            settingsButton.enabled = true;
            apiButton.enabled = true;
        }
    }
}
