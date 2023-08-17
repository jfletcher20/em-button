using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class Selectable : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerDownHandler {
    public Color normalColor;
    public Color hoverColor;
    public Color clickColor;

    private Image image;
    public bool isSelected = false, isEnabled = true;
    public ISelect selectionManager;

    private void Awake() {
        image = GetComponent<Image>();
    }

    public void Initialize(ISelect selectionManager) {
        image.color = normalColor;
        this.selectionManager = selectionManager;
        if (!isEnabled) Disable();
    }

    public void OnPointerEnter(PointerEventData eventData) {
        if (!isSelected && isEnabled)
            image.color = hoverColor;
    }

    public void OnPointerExit(PointerEventData eventData) {
        if (!isSelected && isEnabled)
            image.color = normalColor;
    }

    public void OnPointerDown(PointerEventData eventData) {
        if (!isEnabled) return;
        image.color = clickColor;
        isSelected = true;
        selectionManager.Select(this);
    }

    public void Select() {
        isSelected = true;
        image.color = clickColor;
    }

    public void Deselect() {
        if (!isEnabled) return;
        isSelected = false;
        image.color = normalColor;
    }

    public void Disable() {
        isEnabled = false;
        image.color = Color.clear;
    }

    public void Enable() {
        isEnabled = true;
        image.color = normalColor;
        if (isSelected) Select();
    }
}
