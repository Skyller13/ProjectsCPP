using System.Collections;
using System.Collections.Generic;
using System.Net.Mime;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PressToPlayColor : MonoBehaviour
{
    [SerializeField]Slider slider;

    private Color maxColor;
    private Color minColor;
    private Color selfColor;
    private Color ImageColor;

    private bool way = false;

    private float timer = 0.0f;

    void Start()
    {
        MenuToolbox.Instance.GetManager<MenuManager>();
        MenuToolbox.Instance.GetManager<MenuManager>().loadingScreen = transform.parent.parent.GetChild(2).gameObject;
        MenuToolbox.Instance.GetManager<MenuManager>().slider = slider;
        MenuToolbox.Instance.GetManager<MenuManager>().textTips = slider.transform.GetChild(0).GetComponent<Text>();
        MenuToolbox.Instance.GetManager<MenuManager>().alpha= slider.transform.GetChild(0).GetComponent<CanvasGroup>();

        maxColor = Color.white;
        minColor = new Color(0.39f, 0.39f, 0.39f, 255);
    }

    void Update()
    {
        selfColor = transform.GetComponent<Text>().color;
        ImageColor = transform.GetChild(0).GetComponent<Image>().color;

        timer += Time.deltaTime / 80f;

        if (!way
        && selfColor.r > minColor.r)
        {
            selfColor.r -= timer;
            selfColor.g -= timer;
            selfColor.b -= timer;

            ImageColor.r -= timer;
            ImageColor.g -= timer;
            ImageColor.b -= timer;
        }
        else if (way
        && selfColor.r < maxColor.r)
        {
            selfColor.r += timer;
            selfColor.g += timer;
            selfColor.b += timer;

            ImageColor.r += timer;
            ImageColor.g += timer;
            ImageColor.b += timer;
        }

        if (selfColor.r <= minColor.r
        && !way)
        {
            way = !way;
            timer = 0;
        }

        if (selfColor.r >= maxColor.r
            && way)
        {
            way = !way;
            timer = 0;
        }

        transform.GetChild(0).GetComponent<Image>().color = ImageColor;
        transform.GetComponent<Text>().color = selfColor;


        //all the commentaries need to be change with GameManager activated
        //if (Toolbox.Instance.GetManager<GameManager>().xbox)
        //{
        //transform.GetChild(0).gameObject.SetActive(true);

        if (Input.GetButtonDown("A")
        && transform.parent.parent.GetChild(1).GetChild(0).GetComponent<Image>().color.a == 0)
        {
            transform.parent.gameObject.SetActive(false);
            MenuToolbox.Instance.GetManager<MenuManager>().LoadGame();
            SceneManager.GetSceneByName("Title Screen");
        }

        if (Input.GetKeyDown(KeyCode.Escape)
            || Input.GetButtonDown("Select"))
        {
            Application.Quit();
        }
        //}
        //else
        //{
        //    transform.GetChild(0).gameObject.SetActive(false);
        //    transform.GetComponent<Text>().text = "Press Enter to play";

        //    if (Input.GetKeyDown(KeyCode.Return))
        //    {
        //        SceneManager.LoadScene("TestPathfinding3", LoadSceneMode.Single);
        //    }
        //}
    }
}
