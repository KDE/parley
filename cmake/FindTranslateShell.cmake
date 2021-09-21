find_program (
    TRANS_EXECUTABLE
    trans
    DOC "Translate Shell executable"
)

find_package_handle_standard_args(
    TranslateShell
    REQUIRED_VARS   TRANS_EXECUTABLE
    FAIL_MESSAGE    "Translate Shell application not found"
)
