#pragma once
namespace GenDomainData
{
	//
	public enum class InfoStatus :int {
		Unknown = 0, Normal = 1,
		Tail = 2, Disabled = 4, Info = 8, Inactive = 16,
		Inserted = 32, Updated = 64, Deleted = 128
	};
	public enum class InfoKind :int {
		Unknown = 0, Normal = 1, Modal = 2, Ordinal = 4
	};
	public enum class InfoDataType :int {
		Unknown = 0, Logical = 1, Integer = 2, Real = 4, Text = 8, Other = 16
	};
	//
}// namespace GenDomainData