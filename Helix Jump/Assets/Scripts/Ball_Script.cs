using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball_Script : MonoBehaviour
{
    Material mat;
    Rigidbody rb;

    [SerializeField] GameObject particleObj;

    ParticleSystem ps;
 
    void Start()
    {
        mat = new Material(GetComponent<MeshRenderer>().material);
        rb = GetComponent<Rigidbody>();
        ps = particleObj.GetComponent<ParticleSystem>();
    }

    public void ChangeSkillColor()
    {
        //switch color for more skills
        Color color = mat.color;

        color.r = Random.Range(0f, 1f);
        color.g = Random.Range(0f, 1f);
        color.b = Random.Range(0f, 1f);

        mat.color = color;
        GetComponent<MeshRenderer>().material = mat;
    }

    void Update()
    {
        if (rb.velocity.y > 2.5f)
        {
            Vector3 tempVel = rb.velocity;

            tempVel.y = 2.5f;

            rb.velocity = tempVel;
        }

        if (rb.velocity.y > 0)
        {
            Quaternion tempQuat = particleObj.transform.rotation;

            ParticleSystem.MainModule main = ps.main;

            int particleCount = ps.particleCount;

            ParticleSystem.Particle[] particles = new ParticleSystem.Particle[particleCount];
            ps.GetParticles(particles);

            for (int i = 0; i < particles.Length; i++)
            {
                particles[i].velocity *= rb.velocity.y / 8f;
            }

            ps.SetParticles(particles, particleCount);

            tempQuat = Quaternion.Euler(180, 0, 0);

            particleObj.transform.rotation = tempQuat;
        }
        else if (rb.velocity.y < 0)
        {
            Quaternion tempQuat = particleObj.transform.rotation;

            tempQuat = Quaternion.Euler(0, 0, 0);

            particleObj.transform.rotation = tempQuat;
        }
    }
}
