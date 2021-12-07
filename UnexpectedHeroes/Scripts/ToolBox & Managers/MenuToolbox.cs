using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuToolbox : Singleton<MenuToolbox>
{
    Dictionary<Type, MonoBehaviour> managers = new Dictionary<Type, MonoBehaviour>();

    private void Awake()
    {
        AddManager<MenuManager>();
    }

    public void AddManager<T>() where T : MonoBehaviour
    {
        if (!managers.ContainsKey(typeof(T)))
        {
            GameObject go = new GameObject("@" + typeof(T).Name);
            go.transform.parent = gameObject.transform;
            managers[typeof(T)] = go.AddComponent<T>();
        }
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

}
