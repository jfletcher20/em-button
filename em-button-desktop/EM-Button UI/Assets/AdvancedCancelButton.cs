using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AdvancedCancelButton : MonoBehaviour {

    public APITab apiTab;

    private void Awake() {
        GetComponent<Button>().onClick.AddListener(_enableButtons);
    }

    private void _enableButtons() {
        if (!apiTab.apiButton.enabled) apiTab.apiButton.enabled = true;
    }

}
