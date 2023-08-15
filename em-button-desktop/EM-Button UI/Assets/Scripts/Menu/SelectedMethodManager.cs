using UnityEngine;
using System.Collections.Generic;

public class SelectedMethodManager : MonoBehaviour {
    public List<Selectable> methodObjects;

    private void Start() {
        foreach (Selectable method in methodObjects) {
            method.Initialize();
        }
    }

    public void SelectMethod(Selectable selectedMethod) {
        foreach (Selectable method in methodObjects) {
            if (method == selectedMethod) {
                method.Select();
            } else {
                method.Deselect();
            }
        }
    }
}
