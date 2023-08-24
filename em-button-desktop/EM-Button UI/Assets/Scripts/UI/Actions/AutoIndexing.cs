using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AutoIndexing : MonoBehaviour {
    private Text text;
    private void Awake() {
        text = transform.GetComponentInChildren<Text>();
    }
    private void Update() {
        text.text = (transform.GetSiblingIndex() + 1).ToString();
    }
}
