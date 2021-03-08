"""
 SPDX-FileCopyrightText: 2006-2011 Bryan Tong Minh

 SPDX-License-Identifier: MIT
"""

from mwclient.errors import *  # pylint: disable=unused-import
from mwclient.client import Site, __ver__  # pylint: disable=unused-import
import logging
import warnings

# Show DeprecationWarning
warnings.simplefilter('always', DeprecationWarning)

# Logging: Add a null handler to avoid "No handler found" warnings.
try:
    from logging import NullHandler
except ImportError:
    class NullHandler(logging.Handler):
        def emit(self, record):
            pass

logging.getLogger(__name__).addHandler(NullHandler())
