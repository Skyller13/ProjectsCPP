using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.SceneManagement;

public class AudioSourceContainer
{
    public GameObject SoundingObject;
    public AudioMixerGroup MixerGroup;
    public AudioSource Source;
    public bool IsInTransition = false;
}

[System.Serializable]
public class VolumeData
{
    //Main volume
    [Range(0f,1f)]
    public float Master = 0.5f;
    //Ingame
    [Range(0f, 1f)]
    public float InGame = 0.5f;
    [Range(0f, 1f)]
    public float MainSFX = 0.5f;
    [Range(0f, 1f)]
    public float SecondarySFX = 0.5f;
    [Range(0f, 1f)]
    public float Ambiant = 0.5f;
    [Range(0f, 1f)]
    public float Music = 0.5f;
    //Interface
    [Range(0f, 1f)]
    public float Interface = 0.5f;

    public void UpdateRange()
    {
        Mathf.Clamp(Master, 0f, 1f);
        Mathf.Clamp(InGame, 0f, 1f);
        Mathf.Clamp(MainSFX, 0f, 1f);
        Mathf.Clamp(SecondarySFX, 0f, 1f);
        Mathf.Clamp(Ambiant, 0f, 1f);
        Mathf.Clamp(Music, 0f, 1f);
        Mathf.Clamp(Interface, 0f, 1f);
    }
}

public class SoundManager : MonoBehaviour
{ 
    AudioMixer audioMixer;
    
    AudioSourceContainer[] audioSourcesPool = new AudioSourceContainer[12];
    AudioSourceContainer AmbientSource, MusicSource;

    SoundDataBase dataBase;

    public VolumeData volumeData = new VolumeData();
    VolumeData oldVolumeData = new VolumeData();

    bool isInit = false;
    [SerializeField] string CurrentScene;

    public int PoolSize
    {
        get
        {
            return audioSourcesPool.Length;
        }
    }

    public void Start()
    {
        Init();

        
    }

    private void OnEnable()
    {
        SceneManager.activeSceneChanged += Init;
    }

    private void OnDisable()
    {
        SceneManager.activeSceneChanged -= Init;
    }


    public void Init(Scene arg0, Scene arg1)
    {
        //Config
        ConfigSoundManager configSound = Resources.Load<ConfigSoundManager>("ManagerConfigs/ConfigSoundManager");
        
        ConfigSoundManager.SceneConfig temp = configSound.GetSpecificScene(SceneManager.GetActiveScene().name);
       
        if (configSound != null)
        {
            if (isInit)
            {
                if (temp.musicToPlay != "") ChangeMusic(temp.musicToPlay, 3f);
                if (temp.ambiantToPlay != "") ChangeAmbiance(temp.ambiantToPlay, 3f);
                CurrentScene = temp.sceneName;
            }
            else
            {
                audioMixer = Resources.Load<AudioMixer>("Sounds/MainMixer");
                dataBase = Resources.Load<SoundDataBase>("Sounds/SoundBank");

                for (int i = 0; i < audioSourcesPool.Length; i++)
                {
                    audioSourcesPool[i] = new AudioSourceContainer();
                    audioSourcesPool[i].SoundingObject = new GameObject("AudioSource" + i.ToString());
                    audioSourcesPool[i].SoundingObject.transform.parent = transform;
                    audioSourcesPool[i].Source = audioSourcesPool[i].SoundingObject.AddComponent<AudioSource>();
                }
                AmbientSource = new AudioSourceContainer();
                AmbientSource.SoundingObject = new GameObject("AmbiantAudioSource");
                AmbientSource.SoundingObject.transform.parent = transform;
                AmbientSource.Source = AmbientSource.SoundingObject.AddComponent<AudioSource>();

                MusicSource = new AudioSourceContainer();
                MusicSource.SoundingObject = new GameObject("MusicAudioSource");
                MusicSource.SoundingObject.transform.parent = transform;
                MusicSource.Source = MusicSource.SoundingObject.AddComponent<AudioSource>();

                string tempJSON = File.ReadAllText(Application.dataPath + "/Resources/Sounds/VolumeData.json");
                volumeData = JsonUtility.FromJson<VolumeData>(tempJSON);


                if (temp.musicToPlay != "") Play(temp.musicToPlay);
                if (temp.ambiantToPlay != "") Play(temp.ambiantToPlay);
                CurrentScene = temp.sceneName;
            }
        }

        isInit = true;
    }

    public void Init()
    {
        //Config
        ConfigSoundManager configSound = Resources.Load<ConfigSoundManager>("ManagerConfigs/ConfigSoundManager");

        ConfigSoundManager.SceneConfig temp = configSound.GetSpecificScene(SceneManager.GetActiveScene().name);

        if (configSound != null)
        {
            if (isInit)
            {
                if (temp.musicToPlay != "") ChangeMusic(temp.musicToPlay, 3f);
                if (temp.ambiantToPlay != "") ChangeAmbiance(temp.ambiantToPlay, 3f);
                CurrentScene = temp.sceneName;
            }
            else
            {
                audioMixer = Resources.Load<AudioMixer>("Sounds/MainMixer");
                dataBase = Resources.Load<SoundDataBase>("Sounds/SoundBank");

                for (int i = 0; i < audioSourcesPool.Length; i++)
                {
                    audioSourcesPool[i] = new AudioSourceContainer();
                    audioSourcesPool[i].SoundingObject = new GameObject("AudioSource" + i.ToString());
                    audioSourcesPool[i].SoundingObject.transform.parent = transform;
                    audioSourcesPool[i].Source = audioSourcesPool[i].SoundingObject.AddComponent<AudioSource>();
                }
                AmbientSource = new AudioSourceContainer();
                AmbientSource.SoundingObject = new GameObject("AmbiantAudioSource");
                AmbientSource.SoundingObject.transform.parent = transform;
                AmbientSource.Source = AmbientSource.SoundingObject.AddComponent<AudioSource>();

                MusicSource = new AudioSourceContainer();
                MusicSource.SoundingObject = new GameObject("MusicAudioSource");
                MusicSource.SoundingObject.transform.parent = transform;
                MusicSource.Source = MusicSource.SoundingObject.AddComponent<AudioSource>();

                string tempJSON = File.ReadAllText(Application.dataPath + "/Resources/Sounds/VolumeData.json");
                volumeData = JsonUtility.FromJson<VolumeData>(tempJSON);


                if (temp.musicToPlay != "") Play(temp.musicToPlay);
                if (temp.ambiantToPlay != "") Play(temp.ambiantToPlay);
                CurrentScene = temp.sceneName;
            }
        }

        isInit = true;
    }


    private void Update()
    {
        if (isInit)
        {
            //Volume change cheking to save
            if (volumeData.Master != oldVolumeData.Master ||
                volumeData.InGame != oldVolumeData.InGame ||
                volumeData.MainSFX != oldVolumeData.MainSFX ||
                volumeData.SecondarySFX != oldVolumeData.SecondarySFX ||
                volumeData.Ambiant != oldVolumeData.Ambiant ||
                volumeData.Music != oldVolumeData.Music ||
                volumeData.Interface != oldVolumeData.Interface)
            {
                string serializedVolumes = JsonUtility.ToJson(volumeData, true);
                File.WriteAllText(Application.dataPath + "/Resources/Sounds/VolumeData.json", serializedVolumes);



                oldVolumeData.Master = volumeData.Master;
                oldVolumeData.InGame = volumeData.InGame;
                oldVolumeData.MainSFX = volumeData.MainSFX;
                oldVolumeData.SecondarySFX = volumeData.SecondarySFX;
                oldVolumeData.Ambiant = volumeData.Ambiant;
                oldVolumeData.Music = volumeData.Music;
                oldVolumeData.Interface = volumeData.Interface;
            }
            volumeData.UpdateRange();


            float currentInGameVol = volumeData.InGame * volumeData.Master;
            foreach (var item in audioSourcesPool)
            {
                if (!item.IsInTransition && item.MixerGroup != null)
                {
                    switch (item.MixerGroup.name)
                    {
                        case "Interface":
                            item.Source.volume = volumeData.Interface * volumeData.Master;
                            break;
                        case "Main SFX":
                            item.Source.volume = volumeData.MainSFX * currentInGameVol;
                            break;
                        case "Secondary SFX":
                            item.Source.volume = volumeData.SecondarySFX * currentInGameVol;
                            break;

                        default:
                            item.Source.volume = currentInGameVol;
                            break;
                    }
                }

            }
            if (!MusicSource.IsInTransition)
            {
                MusicSource.Source.volume = volumeData.Music * currentInGameVol;
            }
            if (!AmbientSource.IsInTransition)
            {
                AmbientSource.Source.volume = volumeData.Ambiant * currentInGameVol;
            }

            //Sound spacialisation
            foreach (var item in audioSourcesPool)
            {
                item.Source.gameObject.transform.position = item.SoundingObject.transform.position;
            }

#if UNITY_EDITOR
            if (Input.GetKeyDown(KeyCode.J))
            {
                ChangeMusic("BattleTheme", 3f);
            }
#endif
        }



    }

    /// <summary>
    /// Permet de lancer la lecture d'un son à la condition que la pool d'audioSources ne soit pas pleine, 
    /// Renvoi une reference sur l'audioSource pour un control plus fin si necessaire
    /// </summary>
    /// <param name="nameCode">le code utilisé par la base de donnée des sons (ScriptableSound.code)</param>
    /// <param name="source">le gameobject qui est sensé produire le son</param>
    /// <returns></returns>
    public AudioSource Play(string nameCode, GameObject source = null)
    {
        ScriptedSound currentScriptedSound = dataBase.SoundBank.First(x => x.code == nameCode);
        
        if (currentScriptedSound != null)
        {
            if (currentScriptedSound.mixerGroup.name == "Musics")
            {
                if (MusicSource.Source.isPlaying)
                {
                    MusicSource.Source.Stop();
                }
                MusicSource.Source.clip = currentScriptedSound.clip;
                MusicSource.Source.outputAudioMixerGroup = currentScriptedSound.mixerGroup;
                MusicSource.Source.loop = currentScriptedSound.loop;
                MusicSource.Source.Play();
                StartCoroutine(FadeSoundUp(MusicSource, 3f));
                return MusicSource.Source;

            }
            else if (currentScriptedSound.mixerGroup.name == "Ambiant")
            {
                if (AmbientSource.Source.isPlaying)
                {
                    AmbientSource.Source.Stop();
                }
                AmbientSource.Source.clip = currentScriptedSound.clip;
                AmbientSource.Source.outputAudioMixerGroup = currentScriptedSound.mixerGroup;
                AmbientSource.Source.loop = currentScriptedSound.loop;
                AmbientSource.Source.Play();
                StartCoroutine(FadeSoundUp(MusicSource, 2f));
                return AmbientSource.Source;
            }
            else
            {
                foreach (AudioSourceContainer sourceContainer in audioSourcesPool)
                {
                    if (!sourceContainer.Source.isPlaying)
                    {
                        sourceContainer.SoundingObject = gameObject;
                        sourceContainer.Source.clip = currentScriptedSound.clip;

                        if (source == null)
                        {
                            sourceContainer.Source.transform.position = gameObject.transform.position;
                        }
                        else
                        {
                            sourceContainer.Source.transform.position = source.transform.position;
                        }
                        
                        if (currentScriptedSound.mixerGroup != null)
                        {
                            sourceContainer.Source.outputAudioMixerGroup = currentScriptedSound.mixerGroup;
                        }
                        else
                        {
                            sourceContainer.Source.outputAudioMixerGroup = audioMixer.FindMatchingGroups("Master")[0];
                        }

                        sourceContainer.Source.loop = currentScriptedSound.loop;

                        sourceContainer.Source.Play();
                        return sourceContainer.Source;
                    }
                }
            }
            
        }

        return null;
    }

    IEnumerator FadeSoundUp(AudioSourceContainer _audioSource, float _fadeDuration)
    {
        _audioSource.IsInTransition = true;
        float currentDuration = 0f;
        float initVolume = _audioSource.Source.volume;
        _audioSource.Source.volume = 0f;
        while (currentDuration < 1f)
        {
            currentDuration += Time.deltaTime / _fadeDuration;
            _audioSource.Source.volume = initVolume * currentDuration;
            yield return 0;
        }
        _audioSource.Source.volume = initVolume;
        _audioSource.IsInTransition = false;
    }

    IEnumerator FadeSoundDown(AudioSourceContainer _audioSource, float _fadeDuration)
    {
        _audioSource.IsInTransition = true;
        float currentDuration = 1f;
        float initVolume = _audioSource.Source.volume;
        while (currentDuration > 0f)
        {
            currentDuration -= Time.deltaTime / _fadeDuration;
            _audioSource.Source.volume = initVolume * currentDuration;
            yield return 0;
        }
        _audioSource.Source.volume = initVolume;
        _audioSource.Source.Stop();
        _audioSource.IsInTransition = false;
    }

    IEnumerator TransitionSoundFade(AudioSourceContainer _audioSource, float _fadeDuration, ScriptedSound _nextSound)
    {
        _audioSource.IsInTransition = true;
        float currentDuration = 1f;
        float initVolume = _audioSource.Source.volume;
        while (currentDuration > 0f)
        {
            currentDuration -= Time.deltaTime / (_fadeDuration/2f);
            _audioSource.Source.volume = initVolume * currentDuration;
            yield return 0;
        }
        _audioSource.Source.volume = initVolume;
        _audioSource.Source.Stop();

        _audioSource.Source.clip = _nextSound.clip;
        _audioSource.Source.loop = _nextSound.loop;
        _audioSource.MixerGroup = _nextSound.mixerGroup;
        _audioSource.Source.Play();

        currentDuration = 0f;
        initVolume = _audioSource.Source.volume;
        _audioSource.Source.volume = 0f;
        while (currentDuration < 1f)
        {
            currentDuration += Time.deltaTime / (_fadeDuration / 2f);
            _audioSource.Source.volume = initVolume * currentDuration;
            yield return 0;
        }
        _audioSource.Source.volume = initVolume;
        _audioSource.IsInTransition = false;
    }


    /// <summary>
    /// Met tout les sons en pause à l'exeption de la musique et de l'ambiance
    /// </summary>
    public void PauseAllSounds()
    {
        foreach (AudioSourceContainer sourceContainer in audioSourcesPool)
        {
            if (sourceContainer.Source.isPlaying)
            {
                sourceContainer.Source.Pause();
            }
            
        }
    }


    /// <summary>
    /// Relance la lecture de tout les sons à l'execption de la musique et le l'ambiance
    /// </summary>
    public void ResumeAllSounds()
    {
        foreach (AudioSourceContainer sourceContainer in audioSourcesPool)
        {
            if (!sourceContainer.Source.isPlaying)
            {
                sourceContainer.Source.UnPause();
            }
        }
    }

    /// <summary>
    /// Met en pause uniquement la musique en cours
    /// </summary>
    public void PauseMusic()
    {
        if (MusicSource.Source.isPlaying)
        {
            MusicSource.Source.Pause();
        }
    }

    /// <summary>
    /// Relance uniquement la musique precedement lancée
    /// </summary>
    public void ResumeMusic()
    {
        if (!MusicSource.Source.isPlaying)
        {
            MusicSource.Source.UnPause();
        }
    }

    /// <summary>
    /// Fait une transition progressive entre deux musiques
    /// </summary>
    /// <param name="nameCode">le code utilisé par la base de donnée des sons (ScriptableSound.code) pour la musique suivante</param>
    /// <param name="_fadeDuration">La durée totale de la transition entre les deux musique</param>
    public void ChangeMusic(string _nameCode, float _fadeDuration)
    {
        ScriptedSound _nextSound = dataBase.SoundBank.Find(x => x.code == _nameCode);
        if (_nextSound != null)
        {
            StartCoroutine(TransitionSoundFade(MusicSource, 3f, _nextSound));
        }
        else
        {
            Debug.LogWarning("Error SoundManager.ChangeMusic invalid nameCode no SoundBank match !");
        }
    }

    /// <summary>
    /// Arrete uniquement la musique en cours avec l'option de le faire brutalement ou progressivement
    /// </summary>
    /// <param name="_fade">Si vous voulez que l'arret ce fasse progressivement</param>
    /// <param name="_fadeDuration">En combien de secondes l'arret doit ce faire</param>
    public void StopMusic(bool _fade = true, float _fadeDuration = 3f)
    {
        if (_fade)
        {
            StartCoroutine(FadeSoundDown(MusicSource, _fadeDuration));
        }
        else
        {
            if (MusicSource.Source.isPlaying)
            {
                MusicSource.Source.Stop();
            }
        }
    }

    /// <summary>
    /// Met en pause uniquement l'ambiance en cours
    /// </summary>
    public void PauseAmbiant()
    {
        if (AmbientSource.Source.isPlaying)
        {
            AmbientSource.Source.Pause();
        }
    }

    /// <summary>
    /// Relance uniquement l'ambiance precedement lancée
    /// </summary>
    public void ResumeAmbiant()
    {
        if (!AmbientSource.Source.isPlaying)
        {
            AmbientSource.Source.UnPause();
        }
    }

    /// <summary>
    /// Fait une transition progressive entre deux ambiances
    /// </summary>
    /// <param name="nameCode">le code utilisé par la base de donnée des sons (ScriptableSound.code) pour l'ambiance suivante</param>
    /// <param name="_fadeDuration">La durée totale de la transition entre les deux ambiances</param>
    public void ChangeAmbiance(string _nameCode, float _fadeDuration)
    {
        ScriptedSound _nextSound = dataBase.SoundBank.First(x => x.code == _nameCode);
        if (_nextSound != null)
        {
            StartCoroutine(TransitionSoundFade(AmbientSource, 3f, _nextSound));
        }
        else
        {
            Debug.LogWarning("Error SoundManager.ChangeAmbiance invalid nameCode no SoundBank match !");
        }
    }

    /// <summary>
    /// Arrete uniquement l'ambiance en cours avec l'option de le faire brutalement ou progressivement
    /// </summary>
    /// <param name="_fade">Si vous voulez que l'arret ce fasse progressivement</param>
    /// <param name="_fadeDuration">En combien de secondes l'arret doit ce faire</param>
    public void StopAmbiant(bool _fade = false, float _fadeDuration = 3f)
    {
        if (_fade)
        {
            StartCoroutine(FadeSoundDown(AmbientSource, _fadeDuration));
        }
        else
        {
            if (AmbientSource.Source.isPlaying)
            {
                AmbientSource.Source.Stop();
            }
        }
    }


    
}


