using UnityEngine;

public class ClickAndDragRotate : MonoBehaviour {
    private bool isDragging = false;
    private Vector3 originalPosition;

    private void Awake() {
        originalPosition = transform.rotation.eulerAngles;
    }

    private void Update() {
        transform.LookAt((Input.mousePosition + new Vector3(0, 0, -104)));
    }
}
