using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[System.Serializable] [CreateAssetMenu(fileName = "ConfigInputManager", menuName = "Configs/InputManager")]
public class InputManagerConfig : ScriptableObject
{
    [System.Serializable]
    public class ButtonInputConfig
    {
        public float TapMinDuration = 0.05f;
        public float TapMaxDuration = 0.5f;

        public float PressMinDuration = 0.5f;

        public float ReleaseMinDuration = 0.5f;

        public float HoldMinDuration = 1f;
    }

    public ButtonInputConfig StickLeft = new ButtonInputConfig();
    public ButtonInputConfig StickRight = new ButtonInputConfig();
    public ButtonInputConfig Cross = new ButtonInputConfig();
    public ButtonInputConfig Trigger = new ButtonInputConfig();
    public ButtonInputConfig ButtonA = new ButtonInputConfig();
    public ButtonInputConfig ButtonB = new ButtonInputConfig();
    public ButtonInputConfig ButtonX = new ButtonInputConfig();
    public ButtonInputConfig ButtonY = new ButtonInputConfig();
    public ButtonInputConfig BumperLB = new ButtonInputConfig();
    public ButtonInputConfig BumperRB = new ButtonInputConfig();
    public ButtonInputConfig ButtonStart = new ButtonInputConfig();
    public ButtonInputConfig ButtonBack = new ButtonInputConfig();
    public ButtonInputConfig ButtonStickLeft = new ButtonInputConfig();
    public ButtonInputConfig ButtonStickRight = new ButtonInputConfig();

    [Space(20)]
    public ButtonInputConfig StickLeftKeyboard = new ButtonInputConfig();
    public ButtonInputConfig StickRightKeyboard = new ButtonInputConfig();
    public ButtonInputConfig CrossKeyboard = new ButtonInputConfig();
    public ButtonInputConfig TriggerKeyboard = new ButtonInputConfig();


    [System.Serializable]
    public struct SceneConfig
    {
        public string sceneName;
        public string firstInputMap;
        
    }

    public List<SceneConfig> sceneConfigs = new List<SceneConfig>();

    public SceneConfig GetSpecificScene(string _name)
    {
        Debug.Log(_name);
        return sceneConfigs.First(x => x.sceneName == _name);
    }
}
