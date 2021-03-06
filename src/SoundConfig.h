/*!
 * \brief Sound configuration.
 * \author Thomas Hamboeck, Austrian Kangaroos 2014
 */

#ifndef __AK_SOUND_CONFIG__
#define __AK_SOUND_CONFIG__


/* Robot configuration */
#ifdef I_AM_A_ROBOT
#define NUM_CHANNELS_RX         (2)
#define SOUND_DEVICE_RX         "hw:0,0,0"
#define SOUND_DEVICE_RX_VOL     "default"
#define SOUND_SUBDEVICE_RX      "Left/Right mics"
#define SAMPLE_RATE_RX          (8000)
#else
/* PC configuration */
#define NUM_CHANNELS_RX         (1)
#define SOUND_DEVICE_RX         "default"
#define SOUND_DEVICE_RX_VOL     "default"
#define SOUND_SUBDEVICE_RX      "Capture"
#define SAMPLE_RATE_RX          (8000)
#endif

#define BUFFER_SIZE_RX          (NUM_CHANNELS_RX * 1024)
#define VOLUME_RX               (50 + 1)

#endif
