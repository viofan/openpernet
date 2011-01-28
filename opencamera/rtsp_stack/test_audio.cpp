#include <stdio.h>
#include "../../../opencore-amr-0.1.2/amrnb/interf_enc.h"
#include "../../../opencore-amr-0.1.2/amrnb/interf_dec.h"
#define AUDIO_FILE		"../../media/qlx.pcm"

int main(void)
{
	FILE *fp;
	FILE *decoder_fp;
	int read_num = 0;
	int write_num = 0;
	char buffer[1024] = {0};
	char decoder_buffer[1024] = {0};
	int* fAudioCodec, *fAudioDecodec;
	unsigned char fTo[1024] = {0};

	fAudioCodec = (int*)Encoder_Interface_init(0);
	fAudioDecodec = (int*)Decoder_Interface_init();
	
	fp = fopen(AUDIO_FILE, "rb");
    decoder_fp = fopen("temp.txt", "wb+");
	if (fp < 0 || decoder_fp < 0)
	{
	}
	while((read_num = fread(buffer, 1, 320, fp)))
	{
		Encoder_Interface_Encode(fAudioCodec, MR475, (const short int*)buffer, fTo, 0);
		Decoder_Interface_Decode(fAudioDecodec, (const unsigned char *)fTo, (short int *)decoder_buffer, 0);
		fseek(decoder_fp, write_num, SEEK_SET);
		write_num += read_num;
		fwrite(decoder_buffer, 1, 320, decoder_fp);
		if(read_num < 320)
			break;
	}
	fclose(fp);
	fclose(decoder_fp);
}
