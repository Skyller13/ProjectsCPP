using System.Collections;
using System.Collections.Generic;
using System.Collections.Concurrent;
using UnityEngine;

[System.Serializable]
public class ObjectPool<T> where T : new()
{
    private ConcurrentBag<T> items = new ConcurrentBag<T>();
    private int counter = 0;
    private int Max = 10;

    public bool Add(T item)
    {
        if (counter < Max)
        {
            items.Add(item);
            counter++;
            return true;
        }
        return false;
    }

    public bool Remove(T _item)
    {
        if (items.TryTake(out _item))
        {
            counter--;
            return true;
        }
        else
        {
            return false;
        }
    }

    public void SetMaxPool(int _Max)
    {
        counter = _Max;
    }
}
