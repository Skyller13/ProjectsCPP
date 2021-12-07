using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectileTemplate : MonoBehaviour
{
    GameManager manager;
    SpellData data;
    Node currentNode;
    Vector3 destination;

    float startTime;
    float journeyLength;
    float fractionOfJourney;
    float timerExplode = 0.0f;
    bool hasDamaged;
    bool hasExplode;

    private Unit Target;
    
    void Start()
    {
        manager = Toolbox.Instance.GetManager<GameManager>();

        //set data
        data = GetComponent<Spell>().spellData;

        Toolbox.Instance.GetManager<SoundManager>().Play(data.audioClipCode[0], gameObject);

        currentNode = GetComponent<Spell>().CurrentUnit.GetCurrentNode();

        Target = null;
        startTime = Time.time;
        hasDamaged = false;
        hasExplode = false;

        //audio
        destination = DistanceToUnit();

        destination.y = 1f;
    }

    Vector3 DistanceToUnit()
    {
        Node.NeighborsEnum direction = (Node.NeighborsEnum)GetComponent<Spell>().CurrentUnit.thisDirection;

        if (currentNode != null)
        {
            for (int i = 0; i < data.range; i++)
            {
                Node next = currentNode.GetNeighbor((Node.NeighborsEnum)direction);

                if (next != null)
                {
                    if (!next.bIsWall)
                    {
                        if (next.UnitOnThis != null)
                        {
                            //Monster
                            Target = next.UnitOnThis;
                            return next.UnitOnThis.transform.position;
                        }
                    }
                    else
                    {
                        //wall
                        return currentNode.vPosition;
                    }
                }
                else
                {
                    //explode empty node
                    return currentNode.vPosition;
                }

                currentNode = next;
            }

            //explode if nothing was found before (max Range explosion)
            return currentNode.vPosition;
        }
        else
        {
            //can't cast the spell or explode instant
            return transform.position;
        }
    }

    void Update()
    {
        if (Vector3.Distance(transform.position, destination) >= 0.5f)
        {
            journeyLength = Vector3.Distance(transform.position, destination);

            float distCovered = (Time.time - startTime) * data.spellSpeed / 2f;

            fractionOfJourney = distCovered / journeyLength;

            transform.position = Vector3.Lerp(transform.position, destination, fractionOfJourney);
        }
        else
        {
            //explosion

            transform.Find("Explosion").gameObject.SetActive(true);

            GameObject temp = transform.Find("Explosion").gameObject;

            if (!hasExplode)
            {
                Toolbox.Instance.GetManager<SoundManager>().Play(data.audioClipCode[1], gameObject);
                Toolbox.Instance.GetManager<GameManager>().IncrementTurn();
                hasExplode = true;

                foreach (Transform child in transform)
                {
                    if (child.gameObject != temp)
                    {
                        GameObject.Destroy(child.gameObject);
                    }
                }

                if (GetComponent<Spell>().CurrentUnit.typeUnit == Unit.TypeUnit.Player)
                {
                    Toolbox.Instance.GetManager<GameManager>().GetPlayerUnit().GetComponent<PlayerController>().CallEndAction();
                }

                Destroy(this.gameObject, temp.GetComponentInChildren<ParticleSystem>().main.duration);
            }

            //damages
            if (Target != null
            && !hasDamaged)
            {
                List<Unit> tempList = new List<Unit>();

                tempList.Add(Target);

                Toolbox.Instance.GetManager<SpellManager>().DealDamage(tempList, data, GetComponent<Spell>().CurrentUnit);

                hasDamaged = true;
            }
        }
    }
}
