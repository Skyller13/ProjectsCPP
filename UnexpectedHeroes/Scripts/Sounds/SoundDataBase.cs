using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[CreateAssetMenu(fileName = "New Sound Bank", menuName = "Sound/Create new Sound Bank")] [System.Serializable]
public class SoundDataBase : ScriptableObject
{
   [SerializeField] public List<ScriptedSound> SoundBank = new List<ScriptedSound>();

    public SoundDataBase()
    {

    }
    public SoundDataBase(SoundDataBase _data)
    {
        SoundBank = new List<ScriptedSound>(_data.SoundBank);
    }

    public SoundDataBase(List<ScriptedSound> _list)
    {
        SoundBank = new List<ScriptedSound>(_list);
    }
}
