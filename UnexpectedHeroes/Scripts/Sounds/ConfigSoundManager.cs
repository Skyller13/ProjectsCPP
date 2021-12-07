using System.Linq;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName ="ConfigSoundManager", menuName ="Configs/SoundManager")]
public class ConfigSoundManager : ScriptableObject
{
    [System.Serializable]
   public struct SceneConfig
    {
        public string sceneName;
        public string musicToPlay;
        public string ambiantToPlay;
    }

    [SerializeField] List<SceneConfig> configs = new List<SceneConfig>();

    public SceneConfig GetSpecificScene(string _name)
    {
        return configs.First(x => x.sceneName == _name);
    }
}
