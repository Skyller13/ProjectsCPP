using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BookController : MonoBehaviour
{
    [SerializeField] GameObject[] targets = new GameObject[3];
    [SerializeField] GameObject target;
    [SerializeField] GameObject repulsion;
    int currentTargetID = 0;

    [SerializeField] Animator anim;

    bool shouldRandomAnim = true;
    [SerializeField] bool IsInCombat = false;

    float speed = 0.02f;

    // Start is called before the first frame update
    void Start()
    {
        anim = GetComponent<Animator>();
        

        target = targets[currentTargetID];

        StartCoroutine(UpdateRepulsionToPlayer());
        StartCoroutine(AnimationRandom());
        StartCoroutine(DoBlink());
    }

    // Update is called once per frame
    void Update()
    {
        target = targets[currentTargetID];
        if (IsInCombat) currentTargetID = 0;

        //Rotation control
        Vector3 look = target.transform.position - gameObject.transform.position;
        float dist = look.magnitude;
        float currentSpeed = dist * (speed);

        if (dist > 0.01f)
        {
            if (look != Vector3.zero)
                transform.forward = look.normalized;

            //Velocity control
            Mathf.Clamp(currentSpeed, 0f, 5f);

            transform.position += look.normalized * currentSpeed;
        }
        

    }

    IEnumerator DoBlink()
    {
        while (shouldRandomAnim)
        {
            yield return new WaitForSeconds(Random.Range(1f, 5f));
            anim.SetTrigger("Blink");
            anim.SetInteger("PupilPos", Random.Range(0, 2));
        }
    }


    IEnumerator AnimationRandom()
    {
        bool moveDirection = true;
        while(shouldRandomAnim)
        {
            yield return new WaitForSeconds(Random.Range(7f, 15f));
            float rand = Random.Range(0, 100);
            
            if (rand < 50)
            {
                anim.SetTrigger("Flicker");
            }
            else
            {
                if(!IsInCombat)
                {
                    if (moveDirection)
                    {
                        currentTargetID++;
                        if (currentTargetID > 2)
                        {
                            currentTargetID = 2;
                            moveDirection = !moveDirection;
                        }
                    }
                    else
                    {
                        currentTargetID--;
                        if (currentTargetID < 0)
                        {
                            currentTargetID = 0;
                            moveDirection = !moveDirection;
                        }
                    }
                }
               
                
               
            }
        }
    }

    IEnumerator UpdateRepulsionToPlayer()
    {
        while (gameObject.activeSelf)
        {
            yield return 0;
            //Rotation control
            Vector3 look = gameObject.transform.position - repulsion.transform.position;
            float dist = look.magnitude;
            float currentSpeed = dist * (0.05f);

            if (dist < 0.5f)
            {
                //Velocity control
                Mathf.Clamp(currentSpeed, 0f,2f);

                transform.position += look.normalized * currentSpeed;
            }

        }
        
    }


    public void SetTarget(GameObject[] _target)
    {
        targets = _target;
    }

    public void SetRepulsion(GameObject _target)
    {
        repulsion = _target;
    }
}
