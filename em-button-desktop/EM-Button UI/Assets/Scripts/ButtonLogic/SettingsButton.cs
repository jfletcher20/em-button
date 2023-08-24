using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SettingsButton : MonoBehaviour {

    public APITab apiTab;

    private void Awake() {
        GetComponent<Button>().onClick.AddListener(showSettings);
    }

    private void showSettings() {
        apiTab.initSettingsFormValues();
        apiTab.embModelClickInteractions.disabled = !apiTab.embModelClickInteractions.disabled;
        apiTab.apiButton.enabled = !apiTab.apiButton.enabled;
        apiTab.dataFormAnimator.SetBool("enabled", !apiTab.dataFormAnimator.GetBool("enabled"));
        apiTab.fromSettings = apiTab.dataFormAnimator.GetBool("enabled");
    }
}
