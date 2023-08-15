using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class Selectable : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerDownHandler {
    public Color normalColor;
    public Color hoverColor;
    public Color clickColor;

    private Image image;
    private bool isClicked = false;
    public SelectedMethodManager methodManager;

    private void Awake() {
        image = GetComponent<Image>();
    }

    public void Initialize() {
        image.color = normalColor;
    }

    public void OnPointerEnter(PointerEventData eventData) {
        if (!isClicked)
            image.color = hoverColor;
    }

    public void OnPointerExit(PointerEventData eventData) {
        if (!isClicked)
            image.color = normalColor;
    }

    public void OnPointerDown(PointerEventData eventData) {
        image.color = clickColor;
        isClicked = true;
        methodManager.SelectMethod(this);
    }

    public void Select() {
        isClicked = true;
        image.color = clickColor;
    }

    public void Deselect() {
        isClicked = false;
        image.color = normalColor;
    }
}
