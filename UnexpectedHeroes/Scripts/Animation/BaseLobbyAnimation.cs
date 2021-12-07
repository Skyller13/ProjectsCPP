using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;


public class BaseLobbyAnimation : MonoBehaviour
{
    Animator anim;
    private NavMeshAgent agent;

    // Start is called before the first frame update
    void Start()
    {
        anim = GetComponent<Animator>();
        agent = GetComponent<NavMeshAgent>();
    }

    private void OnDestroy()
    {
    }

    // Update is called once per frame
    void Update()
    {
        SetLoopingAnim();
    }

    void SetLoopingAnim()
    {
        if (agent.velocity.magnitude > 0.1f)
        {
            anim.SetBool("IsWalking", true);
        }
        else
        {
            anim.SetBool("IsWalking", false);
        }
    }
}
