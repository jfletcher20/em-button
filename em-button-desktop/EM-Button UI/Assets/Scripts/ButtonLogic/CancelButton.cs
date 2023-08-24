using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CancelButton : MonoBehaviour {
    public List<Button> buttons;
    private void Awake() {
        GetComponentInChildren<Button>().onClick.AddListener(closeTab);
    }
    public void closeTab() {
        transform.parent.GetComponentInParent<Animator>().SetBool("enabled", false);
        buttons.ForEach((b) => b.enabled = true);
    }
}
