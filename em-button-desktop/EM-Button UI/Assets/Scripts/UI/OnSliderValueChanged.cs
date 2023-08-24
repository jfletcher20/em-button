using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class OnSliderValueChanged : MonoBehaviour {
    private Slider slider;
    private Text sliderValueLabel;

    private void Awake() {
        slider = GetComponentInChildren<Slider>();
        sliderValueLabel = GetComponentInChildren<Text>();
        sliderValueLabel.text = ((int)(slider.value * 100)).ToString() + "%";
    }

    private void Start() {
        slider.onValueChanged.AddListener(UpdateText);
    }

    private void UpdateText(float value) {
        sliderValueLabel.text = ((int)(value * 100)).ToString() + "%";
    }
}
