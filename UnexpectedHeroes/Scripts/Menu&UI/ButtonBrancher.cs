using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ButtonBrancher : MonoBehaviour
{
    public class ButtonScaler
    {
        enum ScaleMode { MatchWidthHeight, IndependantWidthHeight};
        ScaleMode mode;
        Vector2 referenceButtonSize;

        [HideInInspector]
        public Vector2 referenceScreenSize;
        public Vector2 newButtonSize;

        public void Initialize(Vector2 refButtonSize, Vector2 refScreenSize, int scaleMode)
        {
            mode = (ScaleMode)scaleMode;
            referenceButtonSize = refButtonSize;
            referenceScreenSize = refScreenSize;
            SetNewButtonSize();
        }

        void SetNewButtonSize()
        {
            if (mode == ScaleMode.IndependantWidthHeight)
            {
                newButtonSize.x = (referenceButtonSize.x * Screen.width) / referenceScreenSize.x;
                newButtonSize.y = (referenceButtonSize.y * Screen.height) / referenceScreenSize.y;
            }
            else if (mode == ScaleMode.MatchWidthHeight)
            {
                newButtonSize.x = (referenceButtonSize.x * Screen.width) / referenceScreenSize.x;
                newButtonSize.y = newButtonSize.x;
            }
        }
    }

    [System.Serializable]
    public class RevealSettings
    {
        public enum RevealOptions { Linear, Circular };
        public RevealOptions options;
        public float translateSmooth = 5f;
        public float fadeSmooth = 0.01f;
        public bool revealOnStart = false;

        [HideInInspector]
        public bool opening = false;
        [HideInInspector]
        public bool spawned = false;
    }

    [System.Serializable]
    public class LinearSpawner
    {
        public enum RevealStyle { SlideToPosition, FadeInAtPosition };
        public RevealStyle revealStyle;
        public Vector2 direction = new Vector2(0, 1);
        public float baseButtonSpacing = 5f;
        public int buttonNumOffset = 0;

        [HideInInspector]
        public float buttonSpacing = 5f;

        public void FitSpacingToScreenSize(Vector2 refScreenSize)
        {
            float refScreenFloat = (refScreenSize.x + refScreenSize.y) / 2;
            float screenFloat = (Screen.width + Screen.height) / 2;
            buttonSpacing = (baseButtonSpacing * screenFloat) / refScreenFloat;
        }
    }

    [System.Serializable]
    public class CircularSpawner
    {
        public enum RevealStyle { SlideToPosition, FadeInAtPosition };
        public RevealStyle revealStyle;
        public Angle angle;
        public float baseDistFromBrancher = 20;

        [HideInInspector]
        public float distFromBrancher = 0;

        [System.Serializable]
        public struct Angle { public float minAngle; public float maxAngle; }

        public void FitDistanceToScreenSize(Vector2 refScreenSize)
        {
            float refScreenFloat = (refScreenSize.x + refScreenSize.y) / 2;
            float screenFloat = (Screen.width + Screen.height) / 2;
            distFromBrancher = (baseDistFromBrancher * screenFloat) / refScreenFloat;
        }
    }

    public GameObject[] buttonRefs;
    [HideInInspector]
    public List<GameObject> buttons;

    public enum ScaleMode { MatchWidthHeight, IndependantWidthHeight };
    public ScaleMode mode;
    public Vector2 referenceButtonSize;
    public Vector2 referenceScreenSize;

    ButtonScaler buttonScaler = new ButtonScaler();
    public RevealSettings revealSettings = new RevealSettings();
    public LinearSpawner linearSpawner = new LinearSpawner();
    public CircularSpawner circularSpawner = new CircularSpawner();

    float lastScreenWidth = 0;
    float lastScreenHeight = 0;

    void Start()
    {
        buttons = new List<GameObject>();
        buttonScaler = new ButtonScaler();
        lastScreenWidth = Screen.width;
        lastScreenHeight = Screen.height;
        buttonScaler.Initialize(referenceButtonSize, referenceScreenSize, (int)mode);
        circularSpawner.FitDistanceToScreenSize(buttonScaler.referenceScreenSize);
        linearSpawner.FitSpacingToScreenSize(buttonScaler.referenceScreenSize);

        if (revealSettings.revealOnStart)
        {
            SpawnButtons();
        }
    }

    void Update()
    {
        if (Screen.width != lastScreenWidth || Screen.height != lastScreenHeight)
        {
            lastScreenWidth = Screen.width;
            lastScreenHeight = Screen.height;
            buttonScaler.Initialize(referenceButtonSize, referenceScreenSize, (int)mode);
            circularSpawner.FitDistanceToScreenSize(buttonScaler.referenceScreenSize);
            linearSpawner.FitSpacingToScreenSize(buttonScaler.referenceScreenSize);
            SpawnButtons();
        }

        if (revealSettings.opening)
        {
            if (!revealSettings.spawned)
                SpawnButtons();

            switch (revealSettings.options)
            {
                case RevealSettings.RevealOptions.Linear:

                    switch (linearSpawner.revealStyle)
                    {
                        case LinearSpawner.RevealStyle.SlideToPosition: RevealLinearlyNormal();
                            break;
                        case LinearSpawner.RevealStyle.FadeInAtPosition: RevealLinearlyFade();
                            break;
                        default:
                            break;
                    }

                    break;
                case RevealSettings.RevealOptions.Circular:

                    switch (circularSpawner.revealStyle)
                    {
                        case CircularSpawner.RevealStyle.SlideToPosition: //RevealCircularNormal();
                            break;
                        case CircularSpawner.RevealStyle.FadeInAtPosition: //RevealCircularFade();
                            break;
                        default:
                            break;
                    }

                    break;
                default:
                    break;
            }
        }
    }

    public void SpawnButtons()
    {
        revealSettings.opening = true;

        for (int i = buttons.Count - 1; i >= 0; i--)
        {
            Destroy(buttons[i]);
        }
        buttons.Clear();

        ClearCommonButtonBranchers();

        for (int i = 0; i < buttonRefs.Length; i++)
        {
            GameObject b = Instantiate(buttonRefs[i] as GameObject);
            b.transform.SetParent(transform);
            b.transform.position = transform.position;

            if (linearSpawner.revealStyle == LinearSpawner.RevealStyle.FadeInAtPosition || circularSpawner.revealStyle == CircularSpawner.RevealStyle.FadeInAtPosition)
            {
                Color c = b.GetComponent<Image>().color;
                c.a = 0;
                b.GetComponent<Image>().color = c;

                if (b.GetComponentInChildren<Text>())
                {
                    c = b.GetComponentInChildren<Text>().color;
                    c.a = 0;
                    b.GetComponentInChildren<Text>().color = c;
                }
            }
            buttons.Add(b);
        }

        revealSettings.spawned = true;
    }

    void RevealLinearlyNormal()
    {
        for (int i = 0; i < buttons.Count; i++)
        {
            Vector3 targetPos;
            RectTransform buttonRect = buttons[i].GetComponent<RectTransform>();

            buttonRect.sizeDelta = new Vector2(buttonScaler.newButtonSize.x, buttonScaler.newButtonSize.y);

            targetPos.x = linearSpawner.direction.x * ((i + linearSpawner.buttonNumOffset) * (buttonRect.sizeDelta.x + linearSpawner.buttonSpacing)) + transform.position.x;
            targetPos.y = linearSpawner.direction.y * ((i + linearSpawner.buttonNumOffset) * (buttonRect.sizeDelta.y + linearSpawner.buttonSpacing)) + transform.position.y;
            targetPos.z = 0;

            buttonRect.position = Vector3.Lerp(buttonRect.position, targetPos, revealSettings.translateSmooth * Time.deltaTime);
        }
    }

    void RevealLinearlyFade()
    {
        for (int i = 0; i < buttons.Count; i++)
        {
            Vector3 targetPos;
            RectTransform buttonRect = buttons[i].GetComponent<RectTransform>();

            buttonRect.sizeDelta = new Vector2(buttonScaler.newButtonSize.x, buttonScaler.newButtonSize.y);

            targetPos.x = linearSpawner.direction.x * ((i + linearSpawner.buttonNumOffset) * (buttonRect.sizeDelta.x + linearSpawner.buttonSpacing)) + transform.position.x;
            targetPos.y = linearSpawner.direction.y * ((i + linearSpawner.buttonNumOffset) * (buttonRect.sizeDelta.y + linearSpawner.buttonSpacing)) + transform.position.y;
            targetPos.z = 0;

            ButtonFader previousButtonFader;
            if (i > 0)
            {
                previousButtonFader = buttons[i - 1].GetComponent<ButtonFader>();
            }
        }
    }

    void ClearCommonButtonBranchers()
    {
        GameObject[] branchers = GameObject.FindGameObjectsWithTag("ButtonBrancher");
        foreach (GameObject brancher in branchers)
        {
            if (brancher.transform.parent == transform.parent)
            {
                ButtonBrancher bb = brancher.GetComponent<ButtonBrancher>();
                for (int i = bb.buttons.Count - 1; i >= 0; i--)
                {
                    Destroy(bb.buttons[i]);
                }
                bb.buttons.Clear();
            }
        }
    }
}
