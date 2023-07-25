using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MouseInteractions : MonoBehaviour {
    private Material[] defaultMaterials = { };
    public Material hoverMaterial;
    public Material clickMaterial;
    public Animator[] animators;

    private void initMaterials() {
        Renderer[] renderers = gameObject.GetComponentsInChildren<Renderer>();
        defaultMaterials = new Material[renderers.Length];
        int i = 0;
        foreach (Renderer renderer in renderers) defaultMaterials[i++] = renderer.material;
    }
    private void setup() {
        initMaterials();
        if (hoverMaterial == null) hoverMaterial = defaultMaterials[0];
        if (clickMaterial == null) clickMaterial = hoverMaterial;
    }

    void Awake() {
        setup();
    }

    bool mouseOver = false;

    private void OnMouseEnter() {
        mouseOver = true;
        setMaterial(hoverMaterial);
    }

    private void OnMouseExit() {
        mouseOver = false;
        resetMaterial();
    }

    private void OnMouseDown() {
        resetMaterial();
        setMaterial(clickMaterial);
    }

    private void OnMouseUp() {
        if (!mouseOver)
            resetMaterial();
        if (mouseOver)
            setMaterial(hoverMaterial);
    }

    private bool animate = false;
    private void OnMouseUpAsButton() {
        if (animate) {
            animate = false;
            animators[0].SetBool("horizontal", true);
            //animators[0].Play("rotate emcover");
            animators[1].SetBool("enabled", false);
            //animators[1].Play("disable");
        } else {
            animate = true;
            animators[0].SetBool("horizontal", false);
            //animators[0].Play("rotate emcover");
            animators[1].SetBool("enabled", true);
            //animators[1].Play("enable");
        }
    }

    private void setMaterial(Material mat) {
        try {
            GetComponent<Renderer>().material = mat;
        } catch (System.Exception) {
            Renderer[] renderers = GetComponentsInChildren<Renderer>();
            foreach (Renderer item in renderers) {
                item.material = mat;
            }
        }
    }

    private void resetMaterial() {
        Renderer[] renderers = GetComponentsInChildren<Renderer>();
        for (int i = 0; i < renderers.Length - 1 && i < defaultMaterials.Length - 1; i++) {
            renderers[i].material = defaultMaterials[i];
        }
    }
}
