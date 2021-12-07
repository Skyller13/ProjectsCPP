using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

[System.Serializable]
public class ScriptedSound 
{
    public enum typeMixerGroup
    {
        MAIN_SFX = 0,
        SECONDARY_SFX = 1,
        AMBIANT = 2,
        MUSIC = 3,
        INTERFACE = 4
    }

    public string code = "No_name";
    public AudioClip clip;
    public AudioMixerGroup mixerGroup;
    public bool loop = false;

}
