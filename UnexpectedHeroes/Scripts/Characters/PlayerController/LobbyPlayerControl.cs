using System.Collections;
using System.Collections.Generic;
using Kapy.Move;
using UnityEditor;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class LobbyPlayerControl : MonoBehaviour
{
    private LobbyMap map;
    [SerializeField] private Transform[] places;
    [SerializeField] private Text currentText;
    [SerializeField] private GameObject fadeImage;
    private int currentPlace = 0;
    private bool isSwitching = false;
    private bool colorSwitch = false;

    private GameObject previous;
    private GameObject next;
    private GameObject leftJoystick;

    private NavMeshAgent agent;

    private Text previousText;
    private Text nextText;

    //getter

    public int GetCurrentPlace()
    {
        return currentPlace;
    }

    void Start()
    {
        //Toolbox.Instance.GetManager<InputManager>();

        //map = FindObjectOfType<LobbyMap>();

        //if (Toolbox.Instance.GetManager<InputManager>().currentInputMap != map)
        //{
        //    Toolbox.Instance.GetManager<InputManager>().ChangeInputMap(map);
        //}

        ////Inputs
        //map.ONSelect += SelectCurrentMenu;
        //map.ONSwitchMenu += SwitchMenu;   

        previous = GameObject.Find("PreviousArrow").gameObject;
        next = GameObject.Find("NextArrow").gameObject;
        leftJoystick = GameObject.Find("LeftJ").gameObject;

        previousText = GameObject.Find("PreviousAction").GetComponent<Text>();
        nextText = GameObject.Find("NextAction").GetComponent<Text>();

        agent = GetComponent<NavMeshAgent>();
    }

    private void SwitchMenu()
    {
        float selec = Input.GetAxisRaw("Horizontal");

        if (!isSwitching)
        {
            if (selec > 0)
            {
                currentPlace++;

                next.GetComponent<Image>().color = Color.yellow;

                if (SceneManager.GetActiveScene().name == "Lobby")
                {
                    if (currentPlace == 5)
                    {
                        currentPlace = 0;
                    }
                }
                else if (SceneManager.GetActiveScene().name == "Lobby_quest")
                {
                    if (currentPlace == 4)
                    {
                        currentPlace = 0;
                    }
                }

                agent.SetDestination(places[currentPlace].transform.position);

                StartCoroutine(Wait());
            }
            else if (selec < 0)
            {
                currentPlace--;

                previous.GetComponent<Image>().color = Color.yellow;


                if (SceneManager.GetActiveScene().name == "Lobby")
                {
                    if (currentPlace == -1)
                    {
                        currentPlace = 4;
                    }
                }
                else if (SceneManager.GetActiveScene().name == "Lobby_quest")
                {
                    if (currentPlace == -1)
                    {
                        currentPlace = 3;
                    }
                }

                agent.SetDestination(places[currentPlace].transform.position);

                StartCoroutine(Wait());
            }
            else
            {

            }
        }
    }

    private IEnumerator Wait()
    {
        isSwitching = true;

        leftJoystick.GetComponent<Image>().color = Color.grey;

        yield return new WaitForSeconds(0.25f);

        isSwitching = false;

        previous.GetComponent<Image>().color = Color.white;
        next.GetComponent<Image>().color = Color.white;
        leftJoystick.GetComponent<Image>().color = Color.white;
    }

    private IEnumerator SelectMenu()
    {
        currentText.color = Color.yellow;

        yield return new WaitForSeconds(0.3f);

        currentText.color = Color.white;
    }

    private void SelectCurrentMenu()
    {
        if (Input.GetButtonDown("A"))
        {
            StartCoroutine(SelectMenu());

            if (SceneManager.GetActiveScene().name == "Lobby")
            {
                switch (currentPlace)
                {
                    //Cook
                    case 0:
                        break;

                    //Chest
                    case 1:
                        break;

                    //Sleep
                    case 2:
                        fadeImage.GetComponent<Animator>().SetTrigger("FadeOut");
                        //new day
                        break;

                    //Spells
                    case 3:
                        break;

                    //Expedition
                    case 4:
                        fadeImage.GetComponent<Animator>().SetTrigger("FadeOut");
                        //display map => new input map 
                        break;
                }
            }
            else if (SceneManager.GetActiveScene().name == "Lobby_quest")
            {
                switch (currentPlace)
                {
                    //Chest
                    case 0:
                        break;

                    //Board
                    case 1:
                        break;

                    //Expedition
                    case 2:
                        fadeImage.GetComponent<Animator>().SetTrigger("FadeOut");
                        //display map => new input map 
                        break;

                    //Mailbox
                    case 3:
                        break;
                }
            }
        }
    }

    private void UpdateCurrentText()
{
    if (!colorSwitch)
    {
        Color tempColor = currentText.color;

        tempColor.a -= Time.deltaTime / 3f;

        currentText.color = tempColor;

        if (tempColor.a <= 0.15f)
        {
            colorSwitch = true;
        }
    }

    if (colorSwitch)
    {
        Color tempColor = currentText.color;

        tempColor.a += Time.deltaTime / 3f;

        currentText.color = tempColor;

        if (tempColor.a >= 0.5f)
        {
            colorSwitch = false;
        }
    }
}

void OnDestroy()
{
    ////Inputs
    //map.ONSelect -= SelectCurrentMenu;
    //map.ONSwitchMenu -= SwitchMenu;
}

void Update()
{
    SwitchMenu();
    SelectCurrentMenu();
    UpdateCurrentText();

    //switch text
    currentText.text = places[currentPlace].gameObject.name;

    if (SceneManager.GetActiveScene().name == "Lobby")
    {
        if (currentPlace > 0)
        {
            previousText.text = places[currentPlace - 1].gameObject.name;
        }
        else
        {
            previousText.text = places[4].gameObject.name;
        }

        if (currentPlace < 4)
        {
            nextText.text = places[currentPlace + 1].gameObject.name;
        }
        else
        {
            nextText.text = places[0].gameObject.name;
        }
    }
    else if (SceneManager.GetActiveScene().name == "Lobby_quest")
    {
        if (currentPlace > 0)
        {
            previousText.text = places[currentPlace - 1].gameObject.name;
        }
        else
        {
            previousText.text = places[3].gameObject.name;
        }

        if (currentPlace < 3)
        {
            nextText.text = places[currentPlace + 1].gameObject.name;
        }
        else
        {
            nextText.text = places[0].gameObject.name;
        }
    }
}
}
