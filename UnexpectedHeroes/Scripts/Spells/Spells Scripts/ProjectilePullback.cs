using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectilePullback : MonoBehaviour
{
    private Spell spell;

    private Vector3 spawnPos;
    private Vector3 halfDistance;
    private Vector3 destination;
    private bool wayOut;
    private bool wasCalled = false;

    private bool slowBool = false;
    private bool upBool = false;

    float startTime;
    float journeyLength;
    float fractionOfJourney;

    private float minSpeed;
    private float maxSpeed;
    private float currentSpeed;

    private List<Unit> tempList = new List<Unit>(); //this list check if target already took damage

    void Start()
    {
        wayOut = true;

        spell = GetComponent<Spell>();

        spawnPos = transform.position;

        currentSpeed = spell.spellData.spellSpeed;
        maxSpeed = currentSpeed/2f;
        minSpeed = 0.035f;

        startTime = Time.time;

        Node current = spell.CurrentUnit.GetCurrentNode();

        Node next = null;

        for (int i = 0; i < spell.spellData.range; i++)
        {
            if (i == 0)
            {
                next = current.GetNeighbor((Node.NeighborsEnum)spell.CurrentUnit.thisDirection);
            }
            else
            {
                if (next?.GetNeighbor((Node.NeighborsEnum)spell.CurrentUnit.thisDirection) != null)
                {
                    next = next.GetNeighbor((Node.NeighborsEnum)spell.CurrentUnit.thisDirection);
                }
                else
                {
                    i = spell.spellData.range;
                }
            }
        }

        Node half = next;

        destination = new Vector3(half.vPosition.x, 1, half.vPosition.z);
    }

    void Update()
    {
        //change spell speed
        if (slowBool)
        {
            if (currentSpeed > minSpeed)
            {
                currentSpeed -= Time.deltaTime /1.5f;
            }
            else
            {
                currentSpeed = minSpeed;
            }
        }

        if (upBool)
        {
            if (currentSpeed < maxSpeed)
            {
                currentSpeed += Time.deltaTime /3f;
            }
            else
            {
                currentSpeed = maxSpeed;
            }
        }
        //

        if (Vector3.Distance(transform.position, destination) <= 2.5f
        && wayOut)
        {
            slowBool = true;
            upBool = false;
        }

        if (Vector3.Distance(transform.position, destination) <= 2.5f
            && !wayOut)
        {
            //fadeColor

            for (int i = 0; i < transform.childCount; i++)
            {
                Vector3 scale = transform.GetChild(i).GetComponent<Transform>().localScale;

                for (int j = 0; j < transform.GetChild(i).childCount; j++)
                {
                    Vector3 childScale = transform.GetChild(i).GetChild(j).GetComponent<Transform>().localScale;

                    if (childScale.x > 0)
                    {
                        childScale.x -= Time.deltaTime * 3f;
                        childScale.y -= Time.deltaTime * 3f;
                        childScale.z -= Time.deltaTime * 3f;
                    }

                    transform.GetChild(i).GetChild(j).GetComponent<Transform>().localScale = childScale;
                }

                if (scale.x > 0)
                {
                    scale.x -= Time.deltaTime * 3f;
                    scale.y -= Time.deltaTime * 3f;
                    scale.z -= Time.deltaTime * 3f;
                }

                transform.GetChild(i).GetComponent<Transform>().localScale = scale;
            }
        }

        //travel
        if (wayOut)
        {
            if (Vector3.Distance(transform.position, destination) >= 0.15f)
            {
                journeyLength = Vector3.Distance(transform.position, destination);

                float distCovered = (Time.time - startTime) * currentSpeed / 2f;

                fractionOfJourney = distCovered / journeyLength;

                transform.position = Vector3.Lerp(transform.position, destination, fractionOfJourney);
            }
            else
            {
                //switch way

                destination = new Vector3(spawnPos.x, 1, spawnPos.z);

                spell.spellData.CategoryType = SpellData.Category.Imperatif;

                tempList.Clear();

                wayOut = false;
                upBool = true;
                slowBool = false;
            }
        }
        else
        {
            if (Vector3.Distance(transform.position, destination) >= 0.15f)
            {
                journeyLength = Vector3.Distance(transform.position, destination);

                float distCovered = (Time.time - startTime) * currentSpeed / 2f;

                fractionOfJourney = distCovered / journeyLength;

                transform.position = Vector3.Lerp(transform.position, destination, fractionOfJourney);
            }
            else
            {
                //end spell

                if (spell.CurrentUnit.typeUnit == Unit.TypeUnit.Player)
                {
                    spell.CurrentUnit.GetComponent<PlayerController>().CallEndAction();
                }

                Toolbox.Instance.GetManager<GameManager>().IncrementTurn();

                spell.spellData.CategoryType = SpellData.Category.Magical;

                Destroy(this.gameObject);
            }
        }
    }

    void OnTriggerEnter(Collider other)
    {
        Debug.Log(other.name);

        if (other != null
            && other.GetComponent<Unit>()  != null
            && !tempList.Contains(other.GetComponent<Unit>()))
        {
            switch (spell.CurrentUnit.typeUnit)
            {
                case Unit.TypeUnit.Player:

                    if (other.GetComponent<Unit>().typeUnit == Unit.TypeUnit.Enemy)
                    {
                        Toolbox.Instance.GetManager<SpellManager>().DealDamage(other.GetComponent<Unit>(),
                            spell.spellData, spell.CurrentUnit);

                        tempList.Add(other.GetComponent<Unit>());
                    }

                    break;

                case Unit.TypeUnit.Ally:

                    if (other.GetComponent<Unit>().typeUnit == Unit.TypeUnit.Enemy)
                    {
                        Toolbox.Instance.GetManager<SpellManager>().DealDamage(other.GetComponent<Unit>(),
                            spell.spellData, spell.CurrentUnit);

                        tempList.Add(other.GetComponent<Unit>());
                    }

                    break;

                case Unit.TypeUnit.Enemy:

                    if (other.GetComponent<Unit>().typeUnit == Unit.TypeUnit.Player
                        || other.GetComponent<Unit>().typeUnit == Unit.TypeUnit.Ally)
                    {
                        Toolbox.Instance.GetManager<SpellManager>().DealDamage(other.GetComponent<Unit>(),
                            spell.spellData, spell.CurrentUnit);

                        tempList.Add(other.GetComponent<Unit>());
                    }

                    break;
            }
        }
    }

    void OnTriggerExit(Collider other)
    {

    }
}
