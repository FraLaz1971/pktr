#include <fstream>
#include "PacketReader.h"
#include "CCSDSSegmenter.h"
#include "PacketWriter.h"
#include "SwiftHead3Packet.h"

main() {

PacketReader reader(new ifstream("seg_orig.ccsds"), new SwiftHead3Packet() );
CCSDSSegmenter segmenter(80);
segmenter.setNewAPID(14, 15);

PacketWriter writer(new ofstream("seg_split.ccsds") );

PacketPipe pipe;

reader | pipe | segmenter | writer;

pipe.pump();


}
