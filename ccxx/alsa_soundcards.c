#include <alsa/asoundlib.h>
#include <stdio.h>

void error(const char *format, ...);

void print_card_list(void)
{
    int status;
    int card = -1; // use -1 to prime the pump of iterating through card list
    char *longname = NULL;
    char *shortname = NULL;

    if ((status = snd_card_next(&card)) < 0)
    {
        error("cannot determine card number: %s", snd_strerror(status));
        return;
    }
    if (card < 0)
    {
        error("no sound cards found");
        return;
    }
    while (card >= 0)
    {
        printf("Card %d:", card);
        if ((status = snd_card_get_name(card, &shortname)) < 0)
        {
            error("cannot determine card shortname: %s", snd_strerror(status));
            break;
        }
        if ((status = snd_card_get_longname(card, &longname)) < 0)
        {
            error("cannot determine card longname: %s", snd_strerror(status));
            break;
        }
        printf("\tLONG NAME:  %s\n", longname);
        printf("\tSHORT NAME: %s\n", shortname);
        if ((status = snd_card_next(&card)) < 0)
        {
            error("cannot determine card number: %s", snd_strerror(status));
            break;
        }
    }
}

int main()
{
    print_card_list(); // prints a list of all card devices (not only MIDI)
}
