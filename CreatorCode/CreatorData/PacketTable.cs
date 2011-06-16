using System;
using System.Collections.Generic;
using System.Linq;
using System.Data.Linq;
using System.Linq.Dynamic;
using System.Text;

namespace CreatorData
{
    public partial class PacketTable
    {
        public static IList<PacketTable> GetPacketsByConditions(string conditionStrings)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var packets = db.PacketTables.Where(conditionStrings).ToList();
                return packets;
            }
        }

        public static IList<PacketTable> GetPacketsByLicenseName(string licenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (licenseName != string.Empty)
                {
                    return db.PacketTables.Where(c => c.LicenseTable.LicenseName.Equals(licenseName)).OrderBy(d => d.ID).ToList();
                }
                return db.PacketTables.ToList();
            }
        }

        public static bool ValidateVerificationCode(string packetName, string verificationCode)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var packetTables = db.PacketTables.Where(c => c.PacketVerificationCode.Equals(verificationCode) &&
                                                              c.PacketName.Equals(packetName));
                if (packetTables.Count() > 0)
                    return true;
                return false;
            }
        }

        public static PacketTable GetPacketByVerificationCode(string verificationCode)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.PacketTables.Where(c => c.PacketVerificationCode.Equals(verificationCode)).FirstOrDefault();
            }
        }

        public static void CreatePacket(PacketTable packet)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.PacketTables.InsertOnSubmit(packet);
                db.SubmitChanges();
            }
        }

        public static void UpdatePacket(PacketTable packet)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.PacketTables.Attach(packet, true);
                db.SubmitChanges();
            }
        }

        public static void UpdatePackets(IList<PacketTable> packets)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                foreach (PacketTable packet in packets)
                {
                    db.PacketTables.Attach(packet, true);
                    db.SubmitChanges();
                }
            }
        }

        public static void DeletePacket(PacketTable packet)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.PacketTables.Attach(packet, false);
                db.PacketTables.DeleteOnSubmit(packet);
                db.SubmitChanges();
            }
        }   
    }
}
