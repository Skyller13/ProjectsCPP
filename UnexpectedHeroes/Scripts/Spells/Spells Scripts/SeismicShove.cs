using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class SeismicShove : MonoBehaviour
{
    private Spell spell;
    private Unit selfUnit;
    private List<Unit> targetList = new List<Unit>();
    private InspectorSpellAnimation ISA;

    private void Start()
    {
        spell = GetComponent<Spell>();

        selfUnit = spell.CurrentUnit;

        targetList = spell.targetList;

        transform.parent = targetList.First().transform;

        transform.position = targetList.First().transform.position;

        ISA = GetComponent<InspectorSpellAnimation>();

        Toolbox.Instance.GetManager<SoundManager>().Play(spell.spellData.audioClipCode[0], gameObject);

        GetComponent<Animator>().SetTrigger("Start");

        StartCoroutine(Damage());
    }

    private IEnumerator Damage()
    {
        yield return new WaitForSeconds(0.25f);

        Toolbox.Instance.GetManager<SpellManager>().DealDamage(targetList, spell.spellData, selfUnit);
    }

    private void Update()
    {
        if (!ISA.IsAlive)
        {
            if (selfUnit.typeUnit == Unit.TypeUnit.Player)
            {
                selfUnit.GetComponent<PlayerController>().CallEndAction();
            }

            Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

            Destroy(this.gameObject);
        }
    }
}
