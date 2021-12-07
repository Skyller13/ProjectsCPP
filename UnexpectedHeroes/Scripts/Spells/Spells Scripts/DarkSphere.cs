using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class DarkSphere : MonoBehaviour
{
    private Spell spell;
    private Unit selfUnit;
    private List<Unit> targetList = new List<Unit>();

    private void Start()
    {
        spell = GetComponent<Spell>();

        selfUnit = spell.CurrentUnit;

        targetList = spell.targetList;

        transform.parent = targetList[0].transform;

        transform.position = targetList[0].transform.position;

        Toolbox.Instance.GetManager<SoundManager>().Play(spell.spellData.audioClipCode[0], gameObject);

        StartCoroutine(Damage());
    }

    private IEnumerator Damage()
    {
        yield return new WaitForSeconds(0.25f);

        Toolbox.Instance.GetManager<SpellManager>().DealDamage(targetList, spell.spellData, selfUnit);

        if (selfUnit.typeUnit == Unit.TypeUnit.Player)
        {
            selfUnit.GetComponent<PlayerController>().CallEndAction();
        }

        Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

        Destroy(this.gameObject, 2f);
    }

    private void Update()
    {
        
    }
}
