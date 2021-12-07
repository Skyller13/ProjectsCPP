using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Toolbox : Singleton<Toolbox>
{
    Dictionary<Type, MonoBehaviour> managers = new Dictionary<Type, MonoBehaviour>();

    GameManager GM;

    private void Awake()
    {
        GM = AddManager<GameManager>();

        if (SceneManager.GetActiveScene().name == "Lobby")
        {
            InputManager IM = AddManager<InputManager>();
            IM.Init();
        }
        else
        {
            SoundManager SM = AddManager<SoundManager>();

            PathFindingManager PFM = AddManager<PathFindingManager>();
            PFM.KapyStart(GM);

            UIManager UIM = AddManager<UIManager>();
            UIM.KapyStart(GM);

            SpellManager SPM = AddManager<SpellManager>();
            SPM.KapyStart(GM);

            InputManager IM = AddManager<InputManager>();
            IM.KapyStart(GM);
        }
        
    }

    #region Get/Set Functions

    public T AddManager<T>() where T : MonoBehaviour
    {
        if (!managers.ContainsKey(typeof(T)))
        {
            GameObject go = new GameObject("@" + typeof(T).Name);
            go.transform.parent = gameObject.transform;
            managers[typeof(T)] = go.AddComponent<T>();
            
        }
        return managers[typeof(T)] as T;
    }

    public void RemoveManager<T>() where T : MonoBehaviour
    {
        if (managers.ContainsKey(typeof(T)))
        {
            Destroy(managers[typeof(T)].gameObject);
            managers.Remove(typeof(T));
        }
    }

    public T GetManager<T>() where T : MonoBehaviour
    {
        if (managers.ContainsKey(typeof(T)))
        {
            return managers[typeof(T)] as T;
        }
        return null;
    }

    #endregion
}
